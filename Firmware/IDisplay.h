
#ifndef _IDISPLAY_H_
#define _IDISPLAY_H_

#include <Arduino.h>

class IDisplay
{
public:
    virtual void drawFastChar(int16_t x, int16_t y, unsigned char c, uint16_t color, uint16_t bg) = 0;
    virtual void drawFastCharBuffer(unsigned char* buf, uint8_t* colorBuf) = 0; // Assumes full screen
};

#endif
