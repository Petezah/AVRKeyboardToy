
#include "ColorUtils.h"
#include <Arduino.h>

// Pass 8-bit (each) R,G,B, get back 16-bit packed color
uint16_t color565(uint8_t r, uint8_t g, uint8_t b) {
  return ((r & 0xF8) << 8) | ((g & 0xFC) << 3) | (b >> 3);
}

uint16_t lookupColor(uint8_t index)
{
    // C64 style color table
	const char numColors = 16;
	uint8_t colors[numColors * 3] =
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

	// TODO there is something wrong here
	if (index == 14){
		return 0xFFFF;
	} else if (index == 6)
	{
		return 0x001F;
	}
   
    uint8_t* pColor = colors + ((index % numColors) * 3);
    uint16_t color = color565(pColor[0], pColor[1], pColor[2]);
	//Serial.println(color, 16);
 }
