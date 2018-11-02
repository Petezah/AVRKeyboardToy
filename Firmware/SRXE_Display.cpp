/***************************************************
  A shim library on top of Larry's SRXE display functions
 ****************************************************/

#include "SRXE_Display.h"
// #include <limits.h>
// #include "pins_arduino.h"
// #include "wiring_private.h"
// #include <SPI.h>

#include "ColorUtils.h"

inline uint16_t swapcolor(uint16_t x) { 
  return (x << 11) | (x & 0x07E0) | (x >> 11);
}



// Default constructor; simply uses SRXE init functions
SRXE_Display::SRXE_Display() 
  : Adafruit_GFX(LCD_WIDTH, LCD_HEIGHT) {
}

#if defined(CORE_TEENSY) && !defined(__AVR__)
#define __AVR__
#endif


// Initialization SRXE screens
void SRXE_Display::init(void) {
  SRXEInit(0xE7, 0xD6, 0xA2); //CS, D/C, RESET
}

void SRXE_Display::drawPixel(int16_t x, int16_t y, uint16_t color) {

  if((x < 0) ||(x >= _width) || (y < 0) || (y >= _height)) return;

  SRXESetPosition(x, y, 1, 1);
  SRXEWriteDataBlock((uint8_t*)&color, 1);
}


void SRXE_Display::drawFastVLine(int16_t x, int16_t y, int16_t h,
 uint16_t color) {

  // Rudimentary clipping
  if((x >= _width) || (y >= _height)) return;
  if((y+h-1) >= _height) h = _height-y;
  SRXESetPosition(x, y, 1, y+h-1);

  uint8_t hi = color >> 8, lo = color;
  while (h--) {
    SRXEWriteDataBlock((uint8_t*)&color, 1);
  }
}


void SRXE_Display::drawFastHLine(int16_t x, int16_t y, int16_t w,
  uint16_t color) {

  // Rudimentary clipping
  if((x >= _width) || (y >= _height)) return;
  if((x+w-1) >= _width)  w = _width-x;
  SRXESetPosition(x, y, x+w-1, 1);

  uint8_t hi = color >> 8, lo = color;
  while (w--) {
    SRXEWriteDataBlock((uint8_t*)&color, 1);
  }
}

void SRXE_Display::drawFastChar(int16_t x, int16_t y, unsigned char c, uint16_t color, uint16_t bg) {

  // Rudimentary clipping
  if((x >= _width)            || // Clip right
     (y >= _height)           || // Clip bottom
     ((x + 8 - 1) < 0)        || // Clip left
     ((y + 8 - 1) < 0))          // Clip top
     return;

  char sTmp[2] = {c, 0};
  SRXEWriteString(x, y, sTmp, FONT_NORMAL, color, bg);
  return;

  SRXESetPosition(x, y, 8, 8);//8x8 square

  if(!_cp437 && (c >= 176)) c++; // Handle 'classic' charset behavior

  uint8_t hi = color >> 8, lo = color;
  uint8_t bghi = bg >> 8, bglo = bg;

  for(int8_t i=0; i<8; i++ ) {
    // We do not add an artificial blank line;
    // our 8x8 font already has one built-in 
    // where necessary
    uint8_t line = pgm_read_byte(getFont()+(c*8)+i);
    for(int8_t j=0; j<8; j++, line <<= 1) {
      if(line & 0x80) {
        // Draw FG pixel
        SRXEWriteDataBlock((uint8_t*)&color, 1);
      } else {
        // Draw BG pixel
        SRXEWriteDataBlock((uint8_t*)&bg, 1);
      }
    }
  }
}

void SRXE_Display::drawFastCharBuffer(unsigned char* buf, uint8_t* colorBuf)
{
  unsigned char *bufRow = buf;
  uint8_t *colorBufRow = colorBuf;
  char rowTmp[NUM_CHAR_COLUMNS + 1] = {0};
  for(uint8_t y=0; y<NUM_CHAR_ROWS; ++y)
  {
    memcpy(rowTmp, bufRow, NUM_CHAR_COLUMNS);
    SRXEWriteString(0, y*CHAR_HEIGHT, rowTmp, FONT_NORMAL, 3, 0);
    bufRow += NUM_CHAR_COLUMNS;
    colorBufRow += NUM_CHAR_COLUMNS;
  }

#if 0
  uint8_t xMax = CHAR_WIDTH * NUM_CHAR_COLUMNS;
  uint8_t yMax = CHAR_HEIGHT * NUM_CHAR_ROWS;
  SRXESetPosition(0, 0, xMax - 1, yMax - 1);// whole display

  uint8_t lineNum = -1;
  uint8_t charRow = 0;
  unsigned char *bufRow = buf;
  uint8_t *colorBufRow = colorBuf;
  for(uint8_t y=0; y<yMax; ++y)
  {
    ++lineNum;
    if(lineNum >= CHAR_HEIGHT) 
    {
      lineNum = 0;
      ++charRow;
      bufRow += NUM_CHAR_COLUMNS;
      colorBufRow += NUM_CHAR_COLUMNS;
    }

    unsigned char *bufCol = bufRow;
    uint8_t *colorBufCol = colorBufRow;
    for(uint8_t charColumn=0; charColumn<NUM_CHAR_COLUMNS; ++charColumn, ++bufCol, ++colorBufCol)
    {
      unsigned char c = *bufCol;
      uint8_t color = *colorBufCol;
      uint16_t fgColor = lookupColor((color >> 4) & 0x0F);
      uint16_t bgColor = lookupColor(color & 0x0F);
      uint8_t fghi = fgColor >> 8, fglo = fgColor;
      uint8_t bghi = bgColor >> 8, bglo = bgColor;
      uint8_t line = pgm_read_byte(getFont()+(c*8)+lineNum);
      for(int8_t j=0; j<8; j++, line <<= 1) {
        if(line & 0x80) {
          // Draw FG pixel
          SRXEWriteDataBlock((uint8_t*)&fgColor, 1);
        } else {
          // Draw BG pixel
          SRXEWriteDataBlock((uint8_t*)&bgColor, 1);
        }
      }
    }
  }
#endif
}

void SRXE_Display::fillScreen(uint16_t color) {
  fillRect(0, 0,  _width, _height, color);
}

void SRXE_Display::drawFastCharBuffer2xCols(unsigned char* buf, uint8_t* colorBuf)
{
  uint8_t xCols = NUM_CHAR_COLUMNS * 2;
  uint8_t xMax = CHAR_WIDTH * NUM_CHAR_COLUMNS;
  uint8_t yMax = CHAR_HEIGHT * NUM_CHAR_ROWS / 2;
  SRXESetPosition(0, 0, xMax - 1, yMax - 1);// whole display

  uint8_t lineNum = -1;
  uint8_t charRow = 0;
  unsigned char *bufRow = buf;
  uint8_t *colorBufRow = colorBuf;
  for(uint8_t y=0; y<yMax; ++y)
  {
    ++lineNum;
    if(lineNum >= CHAR_HEIGHT) 
    {
      lineNum = 0;
      ++charRow;
      bufRow += xCols;
      colorBufRow += xCols;
    }

    unsigned char *bufCol = bufRow;
    uint8_t *colorBufCol = colorBufRow;
    for(uint8_t charColumn=0; charColumn<xCols; ++charColumn, ++bufCol, ++colorBufCol)
    {
      unsigned char c = *bufCol;
      uint8_t color = *colorBufCol;
      uint16_t fgColor = lookupColor((color >> 4) & 0x0F);
      uint16_t bgColor = lookupColor(color & 0x0F);
      uint16_t lColor = lookupLerpColor((color >> 4) & 0x0F, color & 0x0F);
      uint8_t fghi = fgColor >> 8, fglo = fgColor;
      uint8_t bghi = bgColor >> 8, bglo = bgColor;
      uint8_t lhi = lColor >> 8, llo = lColor;
      uint8_t line = pgm_read_byte(getFont()+(c*8)+lineNum);
      for(int8_t j=0; j<4; j++, line <<= 2) {
        if((line & 0x80) && (line & 0x40)) {
          // Draw FG pixel
          SRXEWriteDataBlock((uint8_t*)&fgColor, 1);
        } else if(!(line & 0x80) && !(line & 0x40)) {
          // Draw BG pixel
          SRXEWriteDataBlock((uint8_t*)&bgColor, 1);
        } else {
          // Draw half-lerp pixel
          SRXEWriteDataBlock((uint8_t*)&lColor, 1);
        }
      }
    }
  }
}

// fill a rectangle
void SRXE_Display::fillRect(int16_t x, int16_t y, int16_t w, int16_t h,
  uint16_t color) {

  // rudimentary clipping (drawChar w/big text requires this)
  if((x >= _width) || (y >= _height)) return;
  if((x + w - 1) >= _width)  w = _width  - x;
  if((y + h - 1) >= _height) h = _height - y;

  SRXESetPosition(0, 0, w, h);

  uint8_t hi = color >> 8, lo = color;
    
  for(y=h; y>0; y--) {
    for(x=w; x>0; x--) {
      SRXEWriteDataBlock((uint8_t*)&color, 1);
    }
  }
}


// Pass 8-bit (each) R,G,B, get back 16-bit packed color
uint16_t SRXE_Display::Color565(uint8_t r, uint8_t g, uint8_t b) {
  return ((r & 0xF8) << 8) | ((g & 0xFC) << 3) | (b >> 3);
}
