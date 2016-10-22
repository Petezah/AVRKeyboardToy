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
#include "TinyBasicPlus.h"

#define AVRKEYTOY_VERSION_STRING "0.5"

class AvrKeyboardToy
{
public:
    AvrKeyboardToy();

    void Init();    
    void Update();

private:
    // Init
    void InitDisplay();
    void InitInput();

    // Update
    void UpdateInput();
    void UpdateInterpreter();
    void UpdateSerial();
    bool UpdateCursor();

    void RefreshDisplay(bool clearOnly);

    bool DispatchSerialInput(char c); // returns true if handled
    void DispatchSpecialKeyboardInput(char c, uint16_t code);
    void DispatchInputChar(char c, uint16_t code);
    void OutputChar(char c);
    void PerformLineTermination();

    // cursor data
    int m_cursorX, m_cursorY;
    bool m_cursorVisible;
    unsigned long m_lastCursorMillis;

    // keyboard
    bool m_keyboardIsActive;

    // interpreter
    BASICRunState m_interpreterState;
};

#endif //_AVRKEYBOARDTOY_H_
