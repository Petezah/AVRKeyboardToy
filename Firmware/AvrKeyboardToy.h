// 
// AVR Keyboard Toy
// 
// Authors: Peter Dunshee <peter@petezah.com>
// 

#ifndef _AVRKEYBOARDTOY_H_
#define _AVRKEYBOARDTOY_H_

#include <Adafruit_GFX.h>
#include <Adafruit_ST7735.h>
#include "DisplayBuffer.h"

class AvrKeyboardToy
{
public:
    AvrKeyboardToy();

    void Init();
    void Update();

private:
    void UpdateInterpreter();
    void UpdateSerial();
    void UpdateCursor();

    void RefreshDisplay(bool clearOnly);
    
    //
    uint16_t m_displayBGcolor;
    uint16_t m_displayFGcolor;

    // cursor data
    int m_cursorX, m_cursorY;
    bool m_cursorVisible;
    unsigned long m_lastCursorMillis;
};

#endif //_AVRKEYBOARDTOY_H_
