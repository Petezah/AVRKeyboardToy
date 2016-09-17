/*
 st7735_virt.c

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

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "sim_time.h"

#include "st7735_virt.h"
#include "avr_spi.h"
#include "avr_ioport.h"

//#define ENABLE_ST7735_DEBUG_OUT
//#define ENABLE_VERBOSE_PIN_DEBUG

#ifdef ENABLE_ST7735_DEBUG_OUT
#define DEBUG_OUT(format, ...) printf(format, ## __VA_ARGS__)
#else
#define DEBUG_OUT(format, ...)
#endif

#ifdef ENABLE_ST7735_VERBOSE_DEBUG_OUT
#define VERBOSE_DEBUG_OUT(format, ...) printf(format, ## __VA_ARGS__)
#else
#define VERBOSE_DEBUG_OUT(format, ...)
#endif

#ifdef ENABLE_VERBOSE_PIN_DEBUG
#define PIN_DEBUG_OUT(format, ...) printf(format, ## __VA_ARGS__)
#else
#define PIN_DEBUG_OUT(format, ...)
#endif

/*
 * Called on the first command byte sent. For setting single
 * byte commands and initiating multi-byte commands.
 */
void
st7735_update_command_register (st7735_t *part)
{
	switch (part->spi_data)
	{
		case ST7735_SWRESET:
			DEBUG_OUT ("ST7735: Software reset\n");
    		break;
		case ST7735_SLPOUT:
			DEBUG_OUT ("ST7735: Out of sleep mode\n");
    		break;
		case ST7735_COLMOD:
			DEBUG_OUT ("ST7735: Set color mode\n");
    		break;
		case ST7735_FRMCTR1:
			DEBUG_OUT ("ST7735: Frame rate ctrl - normal mode\n");
    		break;
		case ST7735_MADCTL:
			DEBUG_OUT ("ST7735: Memory access ctrl (directions)\n");
    		break;
		case ST7735_DISSET5:
			DEBUG_OUT ("ST7735: Display settings #5\n");
    		break;
		case ST7735_INVCTR:
			DEBUG_OUT ("ST7735: Display inversion ctrl\n");
    		break;
		case ST7735_PWCTR1:
			DEBUG_OUT ("ST7735: Power command #1\n");
    		break;
		case ST7735_PWCTR2:
			DEBUG_OUT ("ST7735: Power command #2\n");
    		break;
		case ST7735_PWCTR3:
			DEBUG_OUT ("ST7735: Power command #3\n");
    		break;
		case ST7735_VMCTR1:
			DEBUG_OUT ("ST7735: Power command VMCTR1\n");
    		break;
		case ST7735_PWCTR6:
			DEBUG_OUT ("ST7735: Power command #6\n");
    		break;
		case ST7735_GMCTRP1:
			DEBUG_OUT ("ST7735: Magical unicorn dust\n");
    		break;
		case ST7735_GMCTRN1:
			DEBUG_OUT ("ST7735: Sparkles and rainbows\n");
    		break;
		case ST7735_CASET:
			part->command_register = part->spi_data;
			part->command_register_param_count = 0;
			VERBOSE_DEBUG_OUT ("ST7735: Column addr set\n");
    		break;
		case ST7735_RASET:
			part->command_register = part->spi_data;
			part->command_register_param_count = 0;
			VERBOSE_DEBUG_OUT ("ST7735: Row addr set\n");
    		break;
		case ST7735_NORON:
			DEBUG_OUT ("ST7735: Normal display on\n");
    		break;
		case ST7735_DISPON:
			DEBUG_OUT ("ST7735: Main screen turn on\n");
			st7735_set_flag (part, ST7735_FLAG_DISPLAY_ON, 1);
    		break;
		case ST7735_FRMCTR2:
			DEBUG_OUT ("ST7735: Frame rate control - idle mode\n");
    		break;
		case ST7735_FRMCTR3:
			DEBUG_OUT ("ST7735: Frame rate ctrl - partial mode\n");
    		break;
		case ST7735_PWCTR4:
			DEBUG_OUT ("ST7735: Power command #4\n");
    		break;
		case ST7735_PWCTR5:
			DEBUG_OUT ("ST7735: Power command #5\n");
    		break;
		case ST7735_INVOFF:
			DEBUG_OUT ("ST7735: Don't invert display\n");
			break;
		case ST7735_RAMWR:
			part->command_register = part->spi_data;
			part->command_register_param_count = 0;
			DEBUG_OUT ("ST7735: Writing to VRAM\n");
			break;
		// case SSD1306_VIRT_SET_CONTRAST:
		// 	part->command_register = part->spi_data;
		// 	DEBUG_OUT ("ST7735: CONTRAST SET COMMAND: 0x%02x\n", part->spi_data);
		// 	return;
		// case SSD1306_VIRT_DISP_NORMAL:
		// 	st7735_set_flag (part, ST7735_FLAG_DISPLAY_INVERTED,
		// 	                  0);
		// 	st7735_set_flag (part, ST7735_FLAG_DIRTY, 1);
		// 	DEBUG_OUT ("ST7735: DISPLAY NORMAL\n");
		// 	ST7735_CLEAR_COMMAND_REG(part);
		// 	return;
		// case SSD1306_VIRT_DISP_INVERTED:
		// 	st7735_set_flag (part, ST7735_FLAG_DISPLAY_INVERTED,
		// 	                  1);
		// 	st7735_set_flag (part, ST7735_FLAG_DIRTY, 1);
		// 	DEBUG_OUT ("ST7735: DISPLAY INVERTED\n");
		// 	ST7735_CLEAR_COMMAND_REG(part);
		// 	return;
		// case SSD1306_VIRT_DISP_SUSPEND:
		// 	st7735_set_flag (part, ST7735_FLAG_DISPLAY_ON, 0);
		// 	st7735_set_flag (part, ST7735_FLAG_DIRTY, 1);
		// 	DEBUG_OUT ("ST7735: DISPLAY SUSPENDED\n");
		// 	ST7735_CLEAR_COMMAND_REG(part);
		// 	return;
		// case SSD1306_VIRT_DISP_ON:
		// 	st7735_set_flag (part, ST7735_FLAG_DISPLAY_ON, 1);
		// 	st7735_set_flag (part, ST7735_FLAG_DIRTY, 1);
		// 	DEBUG_OUT ("ST7735: DISPLAY ON\n");
		// 	ST7735_CLEAR_COMMAND_REG(part);
		// 	return;
		// case SSD1306_VIRT_SET_PAGE_START_ADDR
		//                 ... SSD1306_VIRT_SET_PAGE_START_ADDR
		//                                 + SSD1306_VIRT_PAGES - 1:
		// 	part->cursor.page = part->spi_data
		// 	                - SSD1306_VIRT_SET_PAGE_START_ADDR;
		// 	DEBUG_OUT ("ST7735: SET PAGE ADDRESS: 0x%02x\n", part->spi_data);
		// 	ST7735_CLEAR_COMMAND_REG(part);
		// 	return;
		// case SSD1306_VIRT_SET_COLUMN_LOW_NIBBLE
		//                 ... SSD1306_VIRT_SET_COLUMN_LOW_NIBBLE + 0xF:
		// 	part->spi_data -= SSD1306_VIRT_SET_COLUMN_LOW_NIBBLE;
		// 	part->cursor.column = (part->cursor.column & 0xF0)
		// 	                | (part->spi_data & 0xF);
		// 	DEBUG_OUT ("ST7735: SET COLUMN LOW NIBBLE: 0x%02x\n",part->spi_data);
		// 	ST7735_CLEAR_COMMAND_REG(part);
		// 	return;
		// case SSD1306_VIRT_SET_COLUMN_HIGH_NIBBLE
		//                 ... SSD1306_VIRT_SET_COLUMN_HIGH_NIBBLE + 0xF:
		// 	part->spi_data -= SSD1306_VIRT_SET_COLUMN_HIGH_NIBBLE;
		// 	part->cursor.column = (part->cursor.column & 0xF)
		// 	                | ((part->spi_data & 0xF) << 4);
		// 	DEBUG_OUT ("ST7735: SET COLUMN HIGH NIBBLE: 0x%02x\n", part->spi_data);
		// 	ST7735_CLEAR_COMMAND_REG(part);
		// 	return;
		// case SSD1306_VIRT_SET_SEG_REMAP_0:
		// 	st7735_set_flag (part, ST7735_FLAG_SEGMENT_REMAP_0,
		// 	                  1);
		// 	DEBUG_OUT ("ST7735: SET COLUMN ADDRESS 0 TO OLED SEG0 to \n");
		// 	ST7735_CLEAR_COMMAND_REG(part);
		// 	return;
		// case SSD1306_VIRT_SET_SEG_REMAP_127:
		// 	st7735_set_flag (part, ST7735_FLAG_SEGMENT_REMAP_0,
		// 	                  0);
		// 	DEBUG_OUT ("ST7735: SET COLUMN ADDRESS 127 TO OLED SEG0 to \n");
		// 	ST7735_CLEAR_COMMAND_REG(part);
		// 	return;
		// case SSD1306_VIRT_SET_COM_SCAN_NORMAL:
		// 	st7735_set_flag (part, ST7735_FLAG_COM_SCAN_NORMAL,
		// 	                  1);
		// 	DEBUG_OUT ("ST7735: SET COM OUTPUT SCAN DIRECTION NORMAL \n");
		// 	ST7735_CLEAR_COMMAND_REG(part);
		// 	return;
		// case SSD1306_VIRT_SET_COM_SCAN_INVERTED:
		// 	st7735_set_flag (part, ST7735_FLAG_COM_SCAN_NORMAL,
		// 	                  0);
		// 	DEBUG_OUT ("ST7735: SET COM OUTPUT SCAN DIRECTION REMAPPED \n");
		// 	ST7735_CLEAR_COMMAND_REG(part);
		// 	return;
		default:
			// Unknown command
			return;
	}
}

void swap_bytes(uint8_t *byte_pair)
{
	uint8_t tmp = byte_pair[0];
	byte_pair[0] = byte_pair[1];
	byte_pair[1] = tmp;
}

/*
 * Multi-byte command setting
 */
void
st7735_update_setting (st7735_t *part)
{
	switch (part->command_register)
	{
		// case SSD1306_VIRT_SET_CONTRAST:
		// 	part->contrast_register = part->spi_data;
		// 	st7735_set_flag (part, ST7735_FLAG_DIRTY, 1);
		// 	ST7735_CLEAR_COMMAND_REG(part);
		// 	DEBUG_OUT ("ST7735: CONTRAST SET: 0x%02x\n", part->contrast_register);
		// 	return;
		case ST7735_CASET:
			part->window.column.pv[part->command_register_param_count++] = part->spi_data;
			VERBOSE_DEBUG_OUT ("ST7735: Column addr set byte: %d\n", part->spi_data);
			if(part->command_register_param_count >= 4)
			{
				swap_bytes(part->window.column.pv);
				swap_bytes(part->window.column.pv + 2);
				part->cursor.column = part->window.column.p.p1;
				part->window.column.p.p2++; // account for an extra column

				DEBUG_OUT ("ST7735: Column addr set: %d - %d\n", part->window.column.p.p1, part->window.column.p.p2);
				st7735_set_flag (part, ST7735_FLAG_DIRTY, 1);
				ST7735_CLEAR_COMMAND_REG(part);
			}
    		break;
		case ST7735_RASET:
			part->window.row.pv[part->command_register_param_count++] = part->spi_data;
			VERBOSE_DEBUG_OUT ("ST7735: Row addr set byte: %d\n", part->spi_data);
			if(part->command_register_param_count >= 4)
			{
				swap_bytes(part->window.row.pv);
				swap_bytes(part->window.row.pv + 2);
				part->cursor.row = part->window.row.p.p1;
				part->window.row.p.p2++; // account for an extra row

				DEBUG_OUT ("ST7735: Row addr set: %d - %d\n", part->window.row.p.p1, part->window.row.p.p2);
				st7735_set_flag (part, ST7735_FLAG_DIRTY, 1);
				ST7735_CLEAR_COMMAND_REG(part);
			}
    		break;
		case ST7735_RAMWR:
		{
			uint8_t offset = part->command_register_param_count % 2; // Hi or lo byte
			part->vram[part->cursor.row * ST7735_VIRT_STRIDE + part->cursor.column * ST7735_VIRT_BPP + offset] = part->spi_data;

			// Move the cursor
			++part->command_register_param_count;
			if (offset == 1) // if we just stored the second byte, move the cursor
			{
				// Scroll the cursor -- stay within the window
				if (++(part->cursor.column) >= part->window.column.p.p2)
				{
					part->cursor.column = part->window.column.p.p1;
					if (++(part->cursor.row) >= part->window.row.p.p2)
					{
						part->cursor.row = part->window.row.p.p1;
					}
				}
			}
			
			st7735_set_flag (part, ST7735_FLAG_DIRTY, 1);				
			break;
		}
		default:
			// Unknown command
			return;
	}
}

/*
 * Determines whether a new command has been sent
 */
static void
st7735_write_command (st7735_t *part)
{
	// Single byte or start of multi-byte command
	st7735_update_command_register (part);
}

/*
 * Write a byte of data:
 * This can either be pixel data, or we may be in
 * the process of setting a multi-byte command.
 * (The data are the command parameters)
 */
static void
st7735_write_data (st7735_t *part)
{
	// part->vram[part->cursor.page][part->cursor.column] = part->spi_data;

	// // Scroll the cursor
	// if (++(part->cursor.column) >= SSD1306_VIRT_COLUMNS)
	// {
	// 	part->cursor.column = 0;
	// 	if (++(part->cursor.page) >= SSD1306_VIRT_PAGES)
	// 	{
	// 		part->cursor.page = 0;
	// 	}
	// }

	// st7735_set_flag (part, ST7735_FLAG_DIRTY, 1);

	if (!part->command_register)
	{
		// Data; TODO
	} else
	{
		// Multi-byte command setting
		st7735_update_setting (part);
	}
}

/*
 * Called when a SPI byte is sent
 */
static void
st7735_spi_in_hook (struct avr_irq_t * irq, uint32_t value, void * param)
{
	st7735_t * part = (st7735_t*) param;

	// Chip select should be pulled low to enable
	if (part->cs_pin)
		return;

	PIN_DEBUG_OUT ("ST7735: SPI Data in: %d\n", value);
	part->spi_data = value & 0xFF;

	switch (part->di_pin)
	{
		case ST7735_VIRT_DATA:
			st7735_write_data (part);
			break;
		case ST7735_VIRT_COMMAND:
			st7735_write_command (part);
			break;
		default:
			// Invalid value
			break;
	}
}

/*
 * Called when chip select changes
 */
static void
st7735_cs_hook (struct avr_irq_t * irq, uint32_t value, void * param)
{
	st7735_t * p = (st7735_t*) param;
	p->cs_pin = value & 0xFF;
	PIN_DEBUG_OUT ("ST7735: CHIP SELECT:  0x%02x\n", value);

}

/*
 * Called when data/instruction changes
 */
static void
st7735_di_hook (struct avr_irq_t * irq, uint32_t value, void * param)
{
	st7735_t * part = (st7735_t*) param;
	part->di_pin = value & 0xFF;
	PIN_DEBUG_OUT ("ST7735: DATA / INSTRUCTION:  0x%08x\n", value);
}

/*
 * Called when a RESET signal is sent
 */
static void
st7735_reset_hook (struct avr_irq_t * irq, uint32_t value, void * param)
{
	PIN_DEBUG_OUT ("ST7735: RESET\n");
	st7735_t * part = (st7735_t*) param;
	if (irq->value && !value)
	{
		// Falling edge
		memset (part->vram, 0, part->rows * part->pages);
		part->cursor.column = 0;
		part->cursor.row = 0;
		memset (&part->window, 0, sizeof(st7735_virt_window_t));
		part->flags = 0;
		part->command_register = 0x00;
		part->contrast_register = 0x7F;
		st7735_set_flag (part, ST7735_FLAG_COM_SCAN_NORMAL, 1);
		st7735_set_flag (part, ST7735_FLAG_SEGMENT_REMAP_0, 1);
	}

}

static const char * irq_names[IRQ_ST7735_COUNT] =
{ 
	[IRQ_ST7735_SPI_BYTE_IN] = "=st7735.SDIN", 
	[IRQ_ST7735_RESET] = "<st7735.RS", 
	[IRQ_ST7735_DATA_INSTRUCTION] = "<st7735.RW", 
	[IRQ_ST7735_ENABLE] = "<st7735.E",
	[IRQ_ST7735_ADDR] = ">st7735.ADDR"
};

void
st7735_connect (st7735_t * part, st7735_wiring_t * wiring)
{
	avr_connect_irq (
	                avr_io_getirq (part->avr, AVR_IOCTL_SPI_GETIRQ(0),
	                               SPI_IRQ_OUTPUT),
	                part->irq + IRQ_ST7735_SPI_BYTE_IN);

	avr_connect_irq (
	                avr_io_getirq (part->avr,
	                               AVR_IOCTL_IOPORT_GETIRQ(
	                                               wiring->chip_select.port),
	                               wiring->chip_select.pin),
	                part->irq + IRQ_ST7735_ENABLE);

	avr_connect_irq (
	                avr_io_getirq (part->avr,
	                               AVR_IOCTL_IOPORT_GETIRQ(
	                                               wiring->data_instruction.port),
	                               wiring->data_instruction.pin),
	                part->irq + IRQ_ST7735_DATA_INSTRUCTION);

	avr_connect_irq (
	                avr_io_getirq (part->avr,
	                               AVR_IOCTL_IOPORT_GETIRQ(
	                                               wiring->reset.port),
	                               wiring->reset.pin),
	                part->irq + IRQ_ST7735_RESET);
}

void
st7735_init (struct avr_t *avr, struct st7735_t * part, int width, int height)
{
	if (!avr || !part)
		return;

	memset (part, 0, sizeof(*part));
	part->avr = avr;
	part->columns = width;
	part->rows = height;
	part->pages = height / 8; 	// 8 pixels per page

	/*
	 * Register callbacks on all our IRQs
	 */
	part->irq = avr_alloc_irq (&avr->irq_pool, 0, IRQ_ST7735_COUNT,
	                           irq_names);

	avr_irq_register_notify (part->irq + IRQ_ST7735_SPI_BYTE_IN,
	                         st7735_spi_in_hook, part);
	avr_irq_register_notify (part->irq + IRQ_ST7735_RESET,
	                         st7735_reset_hook, part);
	avr_irq_register_notify (part->irq + IRQ_ST7735_ENABLE,
	                         st7735_cs_hook, part);
	avr_irq_register_notify (part->irq + IRQ_ST7735_DATA_INSTRUCTION,
	                         st7735_di_hook, part);

	printf ("ST7735: %duS is %d cycles for your AVR\n", 37,
	        (int) avr_usec_to_cycles (avr, 37));
	printf ("ST7735: %duS is %d cycles for your AVR\n", 1,
	        (int) avr_usec_to_cycles (avr, 1));
}
