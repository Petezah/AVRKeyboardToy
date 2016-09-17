
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
    void setChar(int16_t x, int16_t y, unsigned char c, uint16_t color, uint16_t bg);

    void resolveCursor();
    int16_t getCursorX() { return m_cursor_x; }
    int16_t getCursorY() { return m_cursor_y; }
    void scrollBufferUp();

    char* GetBuffer() { return m_displayBuffer; }
    bool DisplayNeedsRefresh() { return m_displayNeedsRefresh; }
    void ResetRefreshFlag() { m_displayNeedsRefresh = false; }

private:
    IDisplay *m_pOutputDisplay;
    char m_displayBuffer[DISPLAY_BUF_SIZE];
    int16_t m_cursor_x, m_cursor_y;
    bool m_displayNeedsRefresh;
};

#endif //_DISPLAYBUFFER_H_
