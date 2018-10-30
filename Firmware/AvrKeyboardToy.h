// 
// AVR Keyboard Toy
// 
// Authors: Peter Dunshee <peter@petezah.com>
// 

#ifndef _AVRKEYBOARDTOY_H_
#define _AVRKEYBOARDTOY_H_

#include <Adafruit_GFX.h>
#include <SRXE_Display.h>
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
    void DispatchFunctionKeyInput(bool shift, bool ctrl, bool alt, char scanCode);
    void DispatchInputChar(char c, uint16_t code);
    void OutputChar(char c);
    void PerformLineTermination();

    void OutputString(const char* msg, bool newline = false);

    // cursor data
    int m_cursorX, m_cursorY;
    bool m_cursorVisible;
    unsigned long m_lastCursorMillis;
    bool m_displayNeedsRefresh;    
    unsigned long m_lastInputMillis;
    bool m_displayEnabled;

    // keyboard
    bool m_keyboardIsActive;

    // interpreter
    BASICRunState m_interpreterState;
};

#endif //_AVRKEYBOARDTOY_H_
