/*
	uart_stdin.c

	Copyright 2008, 2009 Michel Pollet <buserror@gmail.com>
	Copyright 2016 Peter Dunshee <peterdun@exmsft.com>

 	This file is part of simavr.

	simavr is free software: you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation, either version 3 of the License, or
	(at your option) any later version.

	simavr is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with simavr.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "sim_network.h"
#include <stdlib.h>
#include <pthread.h>
#include <string.h>
#include <stdio.h>
#include <errno.h>
#include <unistd.h>
#include <signal.h>
#include <termios.h>
#ifdef __APPLE__
#include <util.h>
#else
#include <pty.h>
#endif

#include "uart_stdin.h"
#include "avr_uart.h"
#include "sim_time.h"
#include "sim_hex.h"

DEFINE_FIFO(uint8_t,uart_stdin_fifo);

//#define TRACE(_w) _w
#ifndef TRACE
#define TRACE(_w)
#endif

/*
 * called when a byte is send via the uart on the AVR
 */
static void
uart_stdin_in_hook(
		struct avr_irq_t * irq,
		uint32_t value,
		void * param)
{
	uart_stdin_t * p = (uart_stdin_t*)param;
	int stdoutfd = fileno(p->file_out);
	TRACE(printf("uart_stdin_in_hook %02x\n", value);)

	char c = (char)value;
	c = (c == 0x7F) ? 0x08 : c; // convert DELETEs to BKSP prior to printing
	write(stdoutfd, &c, 1);
}

// try to empty our fifo, the uart_stdin_xoff_hook() will be called when
// other side is full
static void
uart_stdin_flush_incoming(
		uart_stdin_t * p)
{
	while (p->xon && !uart_stdin_fifo_isempty(&p->port.out)) {
		TRACE(int r = p->port.out.read;)
		uint8_t byte = uart_stdin_fifo_read(&p->port.out);
		TRACE(printf("uart_stdin_flush_incoming send r %03d:%02x\n", r, byte);)
		avr_raise_irq(p->irq + IRQ_UART_STDIN_BYTE_OUT, byte);
	}
}

avr_cycle_count_t
uart_stdin_flush_timer(
		struct avr_t * avr,
		avr_cycle_count_t when,
		void * param)
{
	uart_stdin_t * p = (uart_stdin_t*)param;

	uart_stdin_flush_incoming(p);
	/* always return a cycle NUMBER not a cycle count */
	return p->xon ? when + avr_hz_to_cycles(p->avr, 1000) : 0;
}

/*
 * Called when the uart has room in it's input buffer. This is called repeateadly
 * if necessary, while the xoff is called only when the uart fifo is FULL
 */
static void
uart_stdin_xon_hook(
		struct avr_irq_t * irq,
		uint32_t value,
		void * param)
{
	uart_stdin_t * p = (uart_stdin_t*)param;
	TRACE(if (!p->xon) printf("uart_stdin_xon_hook\n");)
	p->xon = 1;

	uart_stdin_flush_incoming(p);

	// if the buffer is not flushed, try to do it later
	if (p->xon)
			avr_cycle_timer_register(p->avr, avr_hz_to_cycles(p->avr, 1000),
						uart_stdin_flush_timer, param);
}

/*
 * Called when the uart ran out of room in it's input buffer
 */
static void
uart_stdin_xoff_hook(
		struct avr_irq_t * irq,
		uint32_t value,
		void * param)
{
	uart_stdin_t * p = (uart_stdin_t*)param;
	TRACE(if (p->xon) printf("uart_stdin_xoff_hook\n");)
	p->xon = 0;
	avr_cycle_timer_cancel(p->avr, uart_stdin_flush_timer, param);
}

static struct termios old, new;

/* Initialize new terminal i/o settings */
void initTermios(int echo) 
{
  tcgetattr(0, &old); /* grab old terminal i/o settings */
  new = old; /* make new settings same as old settings */
  new.c_lflag &= ~ICANON; /* disable buffered i/o */
  new.c_lflag &= echo ? ECHO : ~ECHO; /* set echo mode */
  tcsetattr(0, TCSANOW, &new); /* use these new terminal i/o settings now */
}

/* Restore old terminal i/o settings */
void resetTermios(void) 
{
  tcsetattr(0, TCSANOW, &old);
}

/* Read 1 character - echo defines echo mode */
char getch_(int echo) 
{
  char ch;
  initTermios(echo);
  ch = getchar();
  resetTermios();
  return ch;
}

/* Read 1 character without echo */
char getch(void) 
{
  return getch_(0);
}

/* Read 1 character with echo */
char getche(void) 
{
  return getch_(1);
}

static void *
uart_stdin_thread(
		void * param)
{
	uart_stdin_t * p = (uart_stdin_t*)param;

	while(p->running){
		uint8_t c = getch();
		uart_stdin_fifo_write(&p->port.out,
						c);
	}
	return NULL;
}

static const char * irq_names[IRQ_UART_STDIN_COUNT] = {
	[IRQ_UART_STDIN_BYTE_IN] = "8<uart_stdin.in",
	[IRQ_UART_STDIN_BYTE_OUT] = "8>uart_stdin.out",
};

void
uart_stdin_init(
		struct avr_t * avr,
		FILE *output_file,
		uart_stdin_t * p)
{
	memset(p, 0, sizeof(*p));

	p->avr = avr;
	p->irq = avr_alloc_irq(&avr->irq_pool, 0, IRQ_UART_STDIN_COUNT, irq_names);
	avr_irq_register_notify(p->irq + IRQ_UART_STDIN_BYTE_IN, uart_stdin_in_hook, p);

	p->file_out = output_file;

	p->running = 1; // set thread to running state
	pthread_create(&p->thread, NULL, uart_stdin_thread, p);
}

void
uart_stdin_stop(
		uart_stdin_t * p)
{
	puts(__func__);
	pthread_kill(p->thread, SIGINT);

	p->running = 0; // signal thread to exit

	void * ret;
	pthread_join(p->thread, &ret);
}

void
uart_stdin_connect(
		uart_stdin_t * p,
		char uart)
{
	// disable the stdio dump, as we are sending binary there
	uint32_t f = 0;
	avr_ioctl(p->avr, AVR_IOCTL_UART_GET_FLAGS(uart), &f);
	f &= ~AVR_UART_FLAG_STDIO;
	avr_ioctl(p->avr, AVR_IOCTL_UART_SET_FLAGS(uart), &f);

	avr_irq_t * src = avr_io_getirq(p->avr, AVR_IOCTL_UART_GETIRQ(uart), UART_IRQ_OUTPUT);
	avr_irq_t * dst = avr_io_getirq(p->avr, AVR_IOCTL_UART_GETIRQ(uart), UART_IRQ_INPUT);
	avr_irq_t * xon = avr_io_getirq(p->avr, AVR_IOCTL_UART_GETIRQ(uart), UART_IRQ_OUT_XON);
	avr_irq_t * xoff = avr_io_getirq(p->avr, AVR_IOCTL_UART_GETIRQ(uart), UART_IRQ_OUT_XOFF);
	if (src && dst) {
		avr_connect_irq(src, p->irq + IRQ_UART_STDIN_BYTE_IN);
		avr_connect_irq(p->irq + IRQ_UART_STDIN_BYTE_OUT, dst);
	}
	if (xon)
		avr_irq_register_notify(xon, uart_stdin_xon_hook, p);
	if (xoff)
		avr_irq_register_notify(xoff, uart_stdin_xoff_hook, p);
}

int
uart_stdin_inject(
		uart_stdin_t * p,
		uint8_t byte)
{
	TRACE(printf("Inject char %02x into fifo\n", byte));
	return uart_stdin_fifo_write(&p->port.out, byte);
}
