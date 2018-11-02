
#include "ColorUtils.h"
#include <Arduino.h>

// Pass 8-bit (each) R,G,B, get back 16-bit packed color
uint16_t color565(uint8_t r, uint8_t g, uint8_t b) {
  return ((r & 0xF8) << 8) | ((g & 0xFC) << 3) | (b >> 3);
}

#define NUM_C64_COLORS 16
static const uint8_t colors[NUM_C64_COLORS * 3] PROGMEM =
{
	0x00, 0x00, 0x00, //Black
	0xFF, 0xFF, 0xFF, //White
	0x88, 0x00, 0x00, //Red
	0xAA, 0xFF, 0xEE, //Cyan
	0xCC, 0x44, 0xCC, //Violet
	0x00, 0xCC, 0x55, //Green
	0x00, 0x00, 0xAA, //Blue
	0xEE, 0xEE, 0x77, //Yellow
	0xDD, 0x88, 0x55, //Orange
	0x66, 0x44, 0x00, //Brown
	0xFF, 0x77, 0x77, //Lightred
	0x33, 0x33, 0x33, //DarkGray
	0x77, 0x77, 0x77, //MedGray
	0xAA, 0xFF, 0x66, //Lightgreen
	0x00, 0x88, 0xFF, //Lightblue
	0xBB, 0xBB, 0xBB  //Lightgray
};

uint16_t lookupColor(uint8_t index)
{	
    const uint8_t* pColor = colors + ((index % NUM_C64_COLORS) * 3);
	uint8_t r = pgm_read_byte( pColor++ );
	uint8_t g = pgm_read_byte( pColor++ );
	uint8_t b = pgm_read_byte( pColor );
    uint16_t color = color565(r, g, b);

	return b == 0xFF ? 3 : 0;
	//return color;
 }

 uint16_t lookupLerpColor(uint8_t index1, uint8_t index2)
 {
    const uint8_t* pColor = colors + ((index1 % NUM_C64_COLORS) * 3);
	uint8_t r1 = pgm_read_byte( pColor++ );
	uint8_t g1 = pgm_read_byte( pColor++ );
	uint8_t b1 = pgm_read_byte( pColor );
	 
 	pColor = colors + ((index2 % NUM_C64_COLORS) * 3);
	uint8_t r2 = pgm_read_byte( pColor++ );
	uint8_t g2 = pgm_read_byte( pColor++ );
	uint8_t b2 = pgm_read_byte( pColor );

	uint16_t color = color565((r1 >> 1) + (r2 >> 1), (g1 >> 1) + (g2 >> 1), (b1 >> 1) + (b2 >> 1));
	
	return color;
}
