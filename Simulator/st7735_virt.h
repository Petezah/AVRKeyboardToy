/*
 st7735_virt.h

 Copyright 2011 Michel Pollet <buserror@gmail.com>
 Copyright 2014 Doug Szumski <d.s.szumski@gmail.com>
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

/*
 * This "Part" simulates the ST7735 TFT display driver.
 *
 * The following functions are currently supported:
 *
 * > Display reset
 * > Display on / suspend
 * > Setting of the contrast
 * > Inversion of the display
 * > Rotation of the display
 * > Writing to the VRAM using horizontal addressing mode
 *
 * It has been tested on a "JY MCU v1.5 OLED" in 4 wire SPI mode
 * with the E/RD and R/W lines hard wired low as per the datasheet.
 *
 */

#ifndef __ST7735_VIRT_H__
#define __ST7735_VIRT_H__

#include "sim_irq.h"

#define ST7735_VIRT_DATA			1
#define ST7735_VIRT_COMMAND 		0

// Pixel width and heights from Adafruit lib
#define ST7735_TFTWIDTH  128
// for 1.44" display
#define ST7735_TFTHEIGHT_144 128
// for 1.8" display
#define ST7735_TFTHEIGHT_18  160

// VRAM parameters
#define ST7735_VIRT_ROWS	ST7735_TFTHEIGHT_18 // Use max height
#define ST7735_VIRT_COLS	ST7735_TFTWIDTH
#define ST7735_VIRT_BPP		2					// 16 bit color
//
#define ST7735_VIRT_STRIDE  (ST7735_VIRT_COLS * ST7735_VIRT_BPP)

// #define SSD1306_VIRT_PAGES			8
// #define SSD1306_VIRT_COLUMNS			128

// Commands (copied from Adafruit_ST7735 lib)

// Misc
#define ST7735_NOP     0x00  
#define ST7735_SWRESET 0x01	// Software reset
#define ST7735_RDDID   0x04 
#define ST7735_RDDST   0x09

// Sleep commands
#define ST7735_SLPIN   0x10 // In sleep mode
#define ST7735_SLPOUT  0x11 // Out of sleep mode
#define ST7735_PTLON   0x12
#define ST7735_NORON   0x13 // Normal display on

// General display and address setting commands
#define ST7735_INVOFF  0x20 // Don't invert display
#define ST7735_INVON   0x21 // Invert display
#define ST7735_DISPOFF 0x28 // Main screen turn off
#define ST7735_DISPON  0x29 // Main screen turn on
#define ST7735_CASET   0x2A // Column addr set
#define ST7735_RASET   0x2B // Row addr set
#define ST7735_RAMWR   0x2C // write to RAM
#define ST7735_RAMRD   0x2E // read from RAM?

// Color/memory commands
#define ST7735_PTLAR   0x30
#define ST7735_COLMOD  0x3A // Set color mode
#define ST7735_MADCTL  0x36 // Memory access ctrl (directions)

// Frame commands
#define ST7735_FRMCTR1 0xB1 // Frame rate ctrl - normal mode
#define ST7735_FRMCTR2 0xB2 // Frame rate control - idle mode
#define ST7735_FRMCTR3 0xB3 // Frame rate ctrl - partial mode
#define ST7735_INVCTR  0xB4 // Display inversion ctrl
#define ST7735_DISSET5 0xB6 // Display settings #5

// Power commands
#define ST7735_PWCTR1  0xC0
#define ST7735_PWCTR2  0xC1
#define ST7735_PWCTR3  0xC2
#define ST7735_PWCTR4  0xC3
#define ST7735_PWCTR5  0xC4
#define ST7735_VMCTR1  0xC5
#define ST7735_PWCTR6  0xFC

// ?
#define ST7735_RDID1   0xDA
#define ST7735_RDID2   0xDB
#define ST7735_RDID3   0xDC
#define ST7735_RDID4   0xDD

// Unknown, magic number commands
#define ST7735_GMCTRP1 0xE0 // Magical unicorn dust
#define ST7735_GMCTRN1 0xE1 // Sparkles and rainbows

// Memory address control functions (used to set rotation)
#define ST7735_MADCTL_MY  0x80
#define ST7735_MADCTL_MX  0x40
#define ST7735_MADCTL_MV  0x20
#define ST7735_MADCTL_ML  0x10
#define ST7735_MADCTL_RGB 0x00
#define ST7735_MADCTL_BGR 0x08
#define ST7735_MADCTL_MH  0x04

// /* Fundamental commands. */
// #define SSD1306_VIRT_SET_CONTRAST		0x81
// #define SSD1306_VIRT_RESUME_TO_RAM_CONTENT	0xA4
// #define SSD1306_VIRT_IGNORE_RAM_CONTENT		0xA5
// #define SSD1306_VIRT_DISP_NORMAL		0xA6
// #define SSD1306_VIRT_DISP_INVERTED		0xA7
// #define SSD1306_VIRT_DISP_SUSPEND		0xAE
// #define SSD1306_VIRT_DISP_ON			0xAF

// /* Scrolling commands */
// #define SSD1306_VIRT_SCROLL_RIGHT		0x26
// #define SSD1306_VIRT_SCROLL_LEFT		0x27
// #define SSD1306_VIRT_SCROLL_VR			0x29
// #define SSD1306_VIRT_SCROLL_VL			0x2A
// #define SSD1306_VIRT_SCROLL_OFF			0x2E
// #define SSD1306_VIRT_SCROLL_ON   		0x2F
// #define SSD1306_VIRT_VERT_SCROLL_A  		0xA3

// /* Address setting commands */
// #define SSD1306_VIRT_SET_COLUMN_LOW_NIBBLE	0x00
// #define SSD1306_VIRT_SET_COLUMN_HIGH_NIBBLE	0x10
// #define SSD1306_VIRT_MEM_ADDRESSING 		0x20
// #define SSD1306_VIRT_SET_COL_ADDR		0x21
// #define SSD1306_VIRT_SET_PAGE_ADDR		0x22
// #define SSD1306_VIRT_SET_PAGE_START_ADDR	0xB0

// /* Hardware config. commands */
// #define SSD1306_VIRT_SET_LINE			0x40
// #define SSD1306_VIRT_SET_SEG_REMAP_0  		0xA0
// #define SSD1306_VIRT_SET_SEG_REMAP_127		0xA1
// #define SSD1306_VIRT_MULTIPLEX       		0xA8
// #define SSD1306_VIRT_SET_COM_SCAN_NORMAL	0xC0
// #define SSD1306_VIRT_SET_COM_SCAN_INVERTED	0xC8
// #define SSD1306_VIRT_SET_OFFSET			0xD3
// #define SSD1306_VIRT_SET_PADS    		0xDA

// /* Timing & driving scheme setting commands */
// #define SSD1306_VIRT_SET_RATIO_OSC		0xD5
// #define SSD1306_VIRT_SET_CHARGE  		0xD9
// #define SSD1306_VIRT_SET_VCOM    		0xDB
// #define SSD1306_VIRT_NOP     			0xE3

// /* Charge pump command table */
// #define SSD1306_VIRT_CHARGE_PUMP    		0x8D
// #define SSD1306_VIRT_PUMP_ON     		0x14

#define ST7735_CLEAR_COMMAND_REG(part)		part->command_register = 0x00

enum
{
	//IRQ_ST7735_ALL = 0,
	IRQ_ST7735_SPI_BYTE_IN,
	IRQ_ST7735_ENABLE,
	IRQ_ST7735_RESET,
	IRQ_ST7735_DATA_INSTRUCTION,
	//IRQ_ST7735_INPUT_COUNT,
	IRQ_ST7735_ADDR,		// << For VCD
	IRQ_ST7735_COUNT
//TODO: Add IRQs for VCD: Internal state etc.
};

enum
{
	ST7735_FLAG_DISPLAY_INVERTED = 0,
	ST7735_FLAG_DISPLAY_ON,
	ST7735_FLAG_SEGMENT_REMAP_0,
	ST7735_FLAG_COM_SCAN_NORMAL,

	/*
	 * Internal flags, not SSD1306
	 */
	ST7735_FLAG_BUSY,		// 1: Busy between instruction, 0: ready
	ST7735_FLAG_REENTRANT,		// 1: Do not update pins
	ST7735_FLAG_DIRTY,			// 1: Needs redisplay...
};

/*
 * Cursor position in VRAM
 */
struct st7735_virt_cursor_t
{
	uint8_t row;
	uint8_t column;
};

// Drawing coordinate (x extent or y extent)
struct st7735_virt_coord_extent_t
{
	uint16_t p1;
	uint16_t p2;
};

union st7735_virt_coord_t
{
	struct st7735_virt_coord_extent_t p;
	uint8_t pv[4];
};

// Drawing window
typedef struct st7735_virt_window_t
{
	union st7735_virt_coord_t row;
	union st7735_virt_coord_t column;
} st7735_virt_window_t;

typedef struct st7735_t
{
	avr_irq_t * irq;
	struct avr_t * avr;
	uint8_t columns, rows, pages;
	struct st7735_virt_cursor_t cursor;
	struct st7735_virt_window_t window;
	uint8_t vram[ST7735_VIRT_ROWS * ST7735_VIRT_COLS * ST7735_VIRT_BPP];
	uint16_t flags;
	uint8_t command_register;
	uint8_t command_register_param_count;
	uint8_t contrast_register;
	uint8_t cs_pin;
	uint8_t di_pin;
	uint8_t spi_data;
} st7735_t;

typedef struct st7735_pin_t
{
	char port;
	uint8_t pin;
} st7735_pin_t;

typedef struct st7735_wiring_t
{
	st7735_pin_t chip_select;
	st7735_pin_t data_instruction;
	st7735_pin_t reset;
} st7735_wiring_t;

void
st7735_init (struct avr_t *avr, struct st7735_t * b, int width, int height);

static inline int
st7735_set_flag (st7735_t *b, uint16_t bit, int val)
{
	int old = b->flags & (1 << bit);
	b->flags = (b->flags & ~(1 << bit)) | (val ? (1 << bit) : 0);
	return old != 0;
}

static inline int
st7735_get_flag (st7735_t *b, uint16_t bit)
{
	return (b->flags & (1 << bit)) != 0;
}

void
st7735_connect (st7735_t * part, st7735_wiring_t * wiring);

#endif 
