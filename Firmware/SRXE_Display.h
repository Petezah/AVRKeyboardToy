/***************************************************
  A shim library on top of Larry's SRXE display functions
 ****************************************************/

#ifndef _SRXE_DISPLAY_
#define _SRXE_DISPLAY_

#ifndef _WINDOWS
#if ARDUINO >= 100
 #include "Arduino.h"
 #include "Print.h"
#else
 #include "WProgram.h"
#endif
#endif

#include <Adafruit_GFX.h>
#include <IDisplay.h>
#include <SmartResponseXE.h>

#if defined(__SAM3X8E__)
  #include <include/pio.h>
  #define PROGMEM
  #define pgm_read_byte(addr) (*(const unsigned char *)(addr))
  #define pgm_read_word(addr) (*(const unsigned short *)(addr))
  typedef unsigned char prog_uchar;
#elif defined(__AVR__)
  #include <avr/pgmspace.h>
#elif defined(ESP8266)
  #include <pgmspace.h>
#endif

#if defined(__SAM3X8E__)
    #undef __FlashStringHelper::F(string_literal)
    #define F(string_literal) string_literal
#endif

#define CHAR_WIDTH 9
#define CHAR_HEIGHT 8
#define NUM_CHAR_COLUMNS (LCD_WIDTH/CHAR_WIDTH)
#define NUM_CHAR_ROWS    (LCD_HEIGHT/CHAR_HEIGHT)

class SRXE_Display : public Adafruit_GFX, public IDisplay {
  
 public:

  SRXE_Display();

  void     init();
  void     fillScreen(uint16_t color);
  void     drawPixel(int16_t x, int16_t y, uint16_t color);
  void     drawFastVLine(int16_t x, int16_t y, int16_t h, uint16_t color);
  void     drawFastHLine(int16_t x, int16_t y, int16_t w, uint16_t color);
  void     fillRect(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color);
  uint16_t Color565(uint8_t r, uint8_t g, uint8_t b);

  virtual void drawFastChar(int16_t x, int16_t y, unsigned char c, uint16_t color, uint16_t bg);
  virtual void drawFastCharBuffer(unsigned char* buf, uint8_t* colorBuf); // Assumes full screen
  virtual void drawFastCharBuffer2xCols(unsigned char* buf, uint8_t* colorBuf); // Assumes full screen

  // Gains speed by completely overwriting the contents of the block beneath the character
};

#endif
