
#ifndef _DISPLAYBUFFER_H_
#define _DISPLAYBUFFER_H_

 #include "Print.h"
 #include <Adafruit_ST7735.h>
 #include <IDisplay.h>

#define DISPLAY_BUF_SIZE (NUM_CHAR_ROWS*NUM_CHAR_COLUMNS)

class DisplayBuffer : Print
{
public:
    DisplayBuffer(IDisplay *pDisplay);

    virtual size_t write(uint8_t c);
    size_t write(uint8_t c, bool ignoreNewlines);
    void backspace();
    void setChar(int16_t x, int16_t y, unsigned char c, uint8_t color);

    void resolveCursor();
    int16_t getCursorX() { return m_cursor_x; }
    int16_t getCursorY() { return m_cursor_y; }
    void moveCursorUp() { if (m_cursor_y > 0) m_cursor_y--; }
    void moveCursorDown() { if (m_cursor_y < (NUM_CHAR_ROWS - 1)) m_cursor_y++; }
    void moveCursorLeft() { if (m_cursor_x > 0) m_cursor_x--; }
    void moveCursorRight() { if (m_cursor_x < (NUM_CHAR_COLUMNS - 1)) m_cursor_x++; }
    void scrollBufferUp();

    void setBgColor(uint8_t color);
    void setFgColor(uint8_t color);
    void getColors(uint16_t* bg, uint16_t* fg);

    char* GetBuffer() { return m_displayBuffer; }
    uint8_t* GetColorBuffer() { return m_colorBuffer; }
    char* GetCursorLine() { return m_displayBuffer + (NUM_CHAR_COLUMNS * m_cursor_y); }
    bool DisplayNeedsRefresh() { return m_displayNeedsRefresh; }
    void ResetRefreshFlag() { m_displayNeedsRefresh = false; }
    void RefreshDisplay();

private:
    IDisplay *m_pOutputDisplay;
    char m_displayBuffer[DISPLAY_BUF_SIZE];
    uint8_t m_colorBuffer[DISPLAY_BUF_SIZE];
    int16_t m_cursor_x, m_cursor_y;
    uint8_t m_cursorColor; 
    bool m_displayNeedsRefresh;
};

#endif //_DISPLAYBUFFER_H_
