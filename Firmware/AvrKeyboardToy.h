// 
// AVR Keyboard Toy
// 
// Authors: Peter Dunshee <peter@petezah.com>
// 

#ifndef _AVRKEYBOARDTOY_H_
#define _AVRKEYBOARDTOY_H_

#include <Adafruit_GFX.h>
#include <Adafruit_ST7735.h>

#define CHAR_WIDTH 6
#define CHAR_HEIGHT 8
#define DISPLAY_WIDTH 128
#define DISPLAY_HEIGHT 160

#define NUM_CHAR_COLUMNS (DISPLAY_WIDTH/CHAR_WIDTH)
#define NUM_CHAR_ROWS    (DISPLAY_HEIGHT/CHAR_HEIGHT)
#define DISPLAY_BUF_SIZE (NUM_CHAR_ROWS*NUM_CHAR_COLUMNS)

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
