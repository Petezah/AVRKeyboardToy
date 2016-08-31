/*
 st7735_glut.c

 Copyright 2014 Doug Szumski <d.s.szumski@gmail.com>
 Copyright 2016 Peter Dunshee <peterdun@exmsft.com>

 Based on the hd44780 part:
 Copyright Luki <humbell@ethz.ch>
 Copyright 2011 Michel Pollet <buserror@gmail.com>

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
#include "st7735_glut.h"

#if __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

float pix_size_g = 1.0;
float pix_gap_g = 0.0;

void
st7735_gl_init (float pix_size)
{
	pix_size_g = pix_size;
}

// Pass 16-bit, get back R,G,B
void color565_to_rgb(uint16_t color, float *r, float *g, float *b) {
	uint8_t *bytes = (uint8_t*)&color;
	uint8_t temp = bytes[0];
	bytes[0] = bytes[1];
	bytes[1] = temp;

	uint16_t r_unscaled = (color & 0xF800) >> 11;
	uint16_t g_unscaled = (color & 0x07E0) >> 5;
	uint16_t b_unscaled = (color & 0x001F);

	*r = r_unscaled / 31.0f;
	*g = g_unscaled / 63.0f;
	*b = b_unscaled / 31.0f;
}

void
st7735_gl_set_colour (uint16_t color)
{
	float r, g, b;
	color565_to_rgb(color, &r, &g, &b);
	glColor4f (r,
	           g,
	           b,
	           1.0f);
}

uint8_t
st7735_gl_reverse_byte (uint8_t byte)
{
	byte = (byte & 0xF0) >> 4 | (byte & 0x0F) << 4;
	byte = (byte & 0xCC) >> 2 | (byte & 0x33) << 2;
	byte = (byte & 0xAA) >> 1 | (byte & 0x55) << 1;
	return byte;
}

void
st7735_gl_put_pixel_column (uint16_t color)
{
	glEnable (GL_BLEND);
	glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glBegin (GL_QUADS);

	// printf("Putting pixel %d\n", color);
	st7735_gl_set_colour (color);

	for (int i = 0; i < 8; ++i)
	{
		glVertex2f (pix_size_g, pix_size_g * (i + 1));
		glVertex2f (0, pix_size_g * (i + 1));
		glVertex2f (0, pix_size_g * i);
		glVertex2f (pix_size_g, pix_size_g * i);
	}
	glEnd ();
}

/*
 * Controls the mapping between the VRAM and the display.
 */
static uint16_t
st7735_gl_get_vram_pixel (st7735_t *part, uint8_t row, uint8_t column)
{
	// uint8_t seg_remap_default = ssd1306_get_flag (
	//                 part, SSD1306_FLAG_SEGMENT_REMAP_0);
	// uint8_t seg_comscan_default = ssd1306_get_flag (
	//                 part, SSD1306_FLAG_COM_SCAN_NORMAL);

	// if (seg_remap_default && seg_comscan_default)
	// {
	// 	// Normal display
	// 	return part->vram[page][column];
	// } else if (seg_remap_default && !seg_comscan_default)
	// {
	// 	// Normal display, mirrored from upper edge
	// 	return ssd1306_gl_reverse_byte (
	// 	                part->vram[part->pages - 1 - page][column]);
	// }

	// else if (!seg_remap_default && !seg_comscan_default)
	// {
	// 	// Upside down display
	// 	return ssd1306_gl_reverse_byte (
	// 	                part->vram[part->pages - 1 - page][part->columns - 1 - column]);
	// } else if (!seg_remap_default && seg_comscan_default)
	// {
	// 	// Upside down display, mirrored from upper edge
	// 	return part->vram[page][part->columns - 1 - column];
	// }
	
	return *((uint16_t*)&part->vram[row * ST7735_VIRT_STRIDE + column * ST7735_VIRT_BPP]);

	//return 0;
}

static void
st7735_gl_draw_pixels (st7735_t *part)
{
	for (int r = 0; r < ST7735_VIRT_ROWS; r++)
	{
		glPushMatrix ();
		for (int c = 0; c < ST7735_VIRT_COLS; c++)
		{
			uint16_t vram_pixel = st7735_gl_get_vram_pixel (part, r, c);
			st7735_gl_put_pixel_column (vram_pixel);
			// Next column
			glTranslatef (pix_size_g + pix_gap_g, 0, 0);
		}
		glPopMatrix ();
		// Next page
		glTranslatef (0,
		              /* (part->rows / part->pages) * */ pix_size_g + pix_gap_g,
		              0);
	}
}

void
st7735_gl_draw (st7735_t *part)
{
	st7735_set_flag (part, ST7735_FLAG_DIRTY, 0);

	glEnable (GL_BLEND);
	glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glBegin (GL_QUADS);

	// Draw background
	// float opacity = ssd1306_gl_get_pixel_opacity (part->contrast_register);
	// int invert = ssd1306_get_flag (part, SSD1306_FLAG_DISPLAY_INVERTED);
	// st7735_gl_set_colour (invert, opacity);

	glTranslatef (0, 0, 0);
	glBegin (GL_QUADS);
	glVertex2f (ST7735_VIRT_ROWS, 0);
	glVertex2f (0, 0);
	glVertex2f (0, ST7735_VIRT_COLS);
	glVertex2f (ST7735_VIRT_ROWS, ST7735_VIRT_COLS);
	glEnd ();

	// Draw pixels
	if (st7735_get_flag (part, ST7735_FLAG_DISPLAY_ON))
	{
		st7735_gl_draw_pixels (part);
	}
}
