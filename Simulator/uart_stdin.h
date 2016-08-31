/*
	uart_stdin.h

	Copyright 2012 Michel Pollet <buserror@gmail.com>
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


#ifndef __UART_STDIN_H___
#define __UART_STDIN_H___

#include "sim_irq.h"
#include "fifo_declare.h"

enum {
	IRQ_UART_STDIN_BYTE_IN = 0,
	IRQ_UART_STDIN_BYTE_OUT,
	IRQ_UART_STDIN_COUNT
};

DECLARE_FIFO(uint8_t,uart_stdin_fifo, 512);

typedef struct uart_stdin_port_t {
	uart_stdin_fifo_t out;
	uint8_t		buffer[512];
	size_t		buffer_len, buffer_done;
} uart_stdin_port_t, *uart_stdin_port_p;

typedef struct uart_stdin_t {
	avr_irq_t *	irq;		// irq list
	struct avr_t *avr;		// keep it around so we can pause it

	int			running;
	pthread_t	thread;
	int			xon;

	FILE	   *file_out;

	uart_stdin_port_t port;
} uart_stdin_t;

void
uart_stdin_init(
		struct avr_t * avr,
		FILE *output_file,
		uart_stdin_t * b);
void
uart_stdin_stop(uart_stdin_t * p);

void
uart_stdin_connect(
		uart_stdin_t * p,
		char uart);

int
uart_stdin_inject(
		uart_stdin_t * p,
		uint8_t byte);

#endif /* __UART_STDIN_H___ */
