/*
 charlcd.c

 Copyright Luki <humbell@ethz.ch>
 Copyright 2011 Michel Pollet <buserror@gmail.com>
 Copyright 2014 Doug Szumski <d.s.szumski@gmail.com>

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

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <libgen.h>
#include <signal.h>

#include "sim_avr.h"
#include "avr_ioport.h"
#include "sim_elf.h"
#include "sim_gdb.h"
#include "sim_hex.h"

#if __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#include <pthread.h>

#include "st7735_glut.h"
//#include "st7735_virt.h"
#include "uart_stdin.h"

int window_identifier;

avr_t * avr = NULL;
st7735_t st7735;
uart_stdin_t uart_stdin;

static void *
avr_run_thread (void * ignore)
{
	while (1)
	{
		avr_run (avr);
	}
	return NULL;
}

//int bitbanging = 0;

/* Called on a key press */
void
keyCB (unsigned char key, int x, int y)
{
	//printf("keypress %02x\n", key);
	switch (key)
	{
		case 0x1B: // ESC
			exit (0);
			break;
		default:
			uart_stdin_inject(&uart_stdin, key);
			break;
	}
}

/* Function called whenever redisplay needed */

void
displayCB (void)
{
	glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	// Select modelview matrix
	glMatrixMode (GL_MODELVIEW);
	glPushMatrix ();
	// Start with an identity matrix
	glLoadIdentity ();
	st7735_gl_draw (&st7735);
	glPopMatrix ();
	glutSwapBuffers ();
}

// gl timer. if the lcd is dirty, refresh display
void
timerCB (int i)
{
	// restart timer
	glutTimerFunc (1000 / 100, timerCB, 0);
	glutPostRedisplay ();
}

int
initGL (int w, int h, float pix_size)
{
	w *= pix_size;
	h *= pix_size;

	// Double buffered, RGB disp mode.
	glutInitDisplayMode (GLUT_RGB | GLUT_DOUBLE);
	glutInitWindowSize (w * 4, h * 4);
	window_identifier = glutCreateWindow ("ST7735 128x160 TFT");

	// Set up projection matrix
	glMatrixMode (GL_PROJECTION);
	// Start with an identity matrix
	glLoadIdentity ();
	glOrtho (0, w, 0, h, 0, 10);
	glScalef (1, -1, 1);
	glTranslatef (0, -1 * h, 0);

	// Set window's display callback
	glutDisplayFunc (displayCB);
	// Set window's key callback
	glutKeyboardFunc (keyCB);

	glutTimerFunc (1000 / 24, timerCB, 0);

	st7735_gl_init (pix_size);

	return 1;
}

void
sig_int(
	int sign)
{
	printf("signal caught, simavr terminating\n");
	if (avr)
		avr_terminate(avr);
	exit(0);
}

int
main (int argc, char *argv[])
{
	elf_firmware_t f = { { 0 } };
	const char * fname = "AvrKeyboardToy.hex";
	char path[256];
	uint32_t loadBase = AVR_SEGMENT_OFFSET_FLASH;
	sprintf (path, "%s/%s", dirname (argv[0]), fname);
	printf ("Firmware pathname is %s\n", path);
	
	//elf_read_firmware (fname, &f);
	ihex_chunk_p chunk = NULL;
	int cnt = read_ihex_chunks(fname, &chunk);
	if (cnt <= 0) {
		fprintf(stderr, "%s: Unable to load IHEX file %s\n",
			argv[0], fname);
		exit(1);
	}
	printf("Loaded %d section of ihex\n", cnt);
	for (int ci = 0; ci < cnt; ci++)
	{
		if (chunk[ci].baseaddr < (1 * 1024 * 1024)) 
		{
			f.flash = chunk[ci].data;
			f.flashsize = chunk[ci].size;
			f.flashbase = chunk[ci].baseaddr;
			printf("Load HEX flash %08x, %d\n", f.flashbase, f.flashsize);
		}
		else if (chunk[ci].baseaddr >= AVR_SEGMENT_OFFSET_EEPROM ||
			chunk[ci].baseaddr + loadBase >= AVR_SEGMENT_OFFSET_EEPROM)
		{
			// eeprom!
			f.eeprom = chunk[ci].data;
			f.eesize = chunk[ci].size;
			printf("Load HEX eeprom %08x, %d\n", chunk[ci].baseaddr, f.eesize);
		}
	}

	// Hard-coded frequency and mcu
	strcpy(f.mmcu, "atmega328");
	f.frequency = 8000000;

	printf ("firmware %s f=%d mmcu=%s\n", fname, (int) f.frequency, f.mmcu);

	avr = avr_make_mcu_by_name (f.mmcu);
	if (!avr)
	{
		fprintf (stderr, "%s: AVR '%s' not known\n", argv[0], f.mmcu);
		exit (1);
	}

	printf("init avr\n");
	avr_init (avr);
	printf("loading avr firmware\n");
	avr_load_firmware (avr, &f);

	st7735_init (avr, &st7735, 128, 64);

	// ST7735 wired to the SPI bus, with the following additional pins:
	// CS=10   -> PB2
	// RST=9   -> PB1
	// DC=8    -> PB0
	// SCK=13  -> PB5
	// MOSI=11 -> PB3
	st7735_wiring_t wiring =
	{
		.chip_select.port = 'B',
		.chip_select.pin = 2,
		.data_instruction.port = 'B',
		.data_instruction.pin = 0,
		.reset.port = 'B',
		.reset.pin = 1,
	};

	st7735_connect (&st7735, &wiring);

	// PS/2 keyboard is wired to CLK->D3, DATA->D4;
	// At some point we can revisit that; for now
	// we inject over virtual UART
	uart_stdin_init(avr, stdout, &uart_stdin);
	uart_stdin_connect(&uart_stdin, '0');

	printf ("ST7735 display demo\n   Press 'q' to quit\n");

	// Initialize GLUT system
	glutInit (&argc, argv);
	initGL (ST7735_VIRT_COLS, ST7735_VIRT_ROWS, 0.5);// (ssd1306.columns, ssd1306.rows, 0.5);

	// Trace settings, if necessary
    //avr->trace = 1;
	avr->log = 3;

	pthread_t run;
	pthread_create (&run, NULL, avr_run_thread, NULL);

	glutMainLoop ();

	// even if not setup at startup, activate gdb if crashing
	//avr->gdb_port = 1234;
	//if (gdb) {
	//	avr->state = cpu_Stopped;
	//	avr_gdb_init(avr);
	//}

	// signal(SIGINT, sig_int);
	// signal(SIGTERM, sig_int);

	// for (;;) {
	// 	int state = avr_run(avr);
	// 	if (state == cpu_Done || state == cpu_Crashed)
	// 		break;
	// }

	// avr_terminate(avr);
}
