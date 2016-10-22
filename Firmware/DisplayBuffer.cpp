
#include "DisplayBuffer.h"
#include "ColorUtils.h"

#define DEFAULT_CURSOR_COLOR 0xE6

DisplayBuffer::DisplayBuffer(IDisplay *pDisplay) :
    m_pOutputDisplay(pDisplay), m_cursor_x(0), m_cursor_y(0),
    m_cursorColor(DEFAULT_CURSOR_COLOR), m_displayNeedsRefresh(false)
{
    memset(m_displayBuffer, ' ', sizeof(m_displayBuffer));
    memset(m_colorBuffer, DEFAULT_CURSOR_COLOR, sizeof(m_colorBuffer)); // bg color 14, fg color 6
}

size_t DisplayBuffer::write(uint8_t c)
{
    return write(c, false);
}

size_t DisplayBuffer::write(uint8_t c, bool ignoreNewlines)
{
    if(!ignoreNewlines && c == '\n') 
    {
        m_cursor_x = 0;
        m_cursor_y ++;
        resolveCursor();
    } 
    else if(!ignoreNewlines && c == '\r') 
    {
        // skip
    } 
    else 
    {
        resolveCursor();
        setChar(m_cursor_x, m_cursor_y, c, m_cursorColor);
        m_cursor_x ++;
        resolveCursor();
    }
}

void DisplayBuffer::backspace()
{
    if (m_cursor_x > 0)
    {
        m_cursor_x --;
        setChar(m_cursor_x, m_cursor_y, ' ', m_cursorColor);
    }
}

void DisplayBuffer::setChar(int16_t x, int16_t y, unsigned char c, uint8_t color)
{
    uint8_t displayIdx = y*NUM_CHAR_COLUMNS + x; 
    m_displayBuffer[displayIdx] = c;
    m_colorBuffer[displayIdx] = color;
    if(m_pOutputDisplay != NULL)
    {
        //Serial.println((int)color, 16);
        uint16_t fg = lookupColor((color >> 4) & 0x0F);
        uint16_t bg = lookupColor(color & 0x0F);

        m_pOutputDisplay->drawFastChar(x*CHAR_WIDTH, y*CHAR_HEIGHT, c, fg, bg);
    }
}

void DisplayBuffer::resolveCursor()
{
    if(m_cursor_x >= NUM_CHAR_COLUMNS)
    { // Over the edge
        m_cursor_x = 0;   // Reset x to zero
        m_cursor_y ++;    // Advance y one line_terminator
    }

    if(m_cursor_y >= NUM_CHAR_ROWS)
    { // Off the bottom
        m_cursor_y --;     // back up one line
        scrollBufferUp();
    }
}

void DisplayBuffer::scrollBufferUp()
{
    char *pC = m_displayBuffer;
    for (uint16_t y=0; y<NUM_CHAR_ROWS-1; ++y, pC += NUM_CHAR_COLUMNS)
    {
        memcpy(pC, pC + NUM_CHAR_COLUMNS, NUM_CHAR_COLUMNS);
    }
    
    // Blank out the last line after scrolling
    memset(pC, ' ', NUM_CHAR_COLUMNS);

    if (m_pOutputDisplay != NULL)
    {
        m_pOutputDisplay->drawFastCharBuffer((unsigned char*)m_displayBuffer, m_colorBuffer);
    }
    else
    {
        m_displayNeedsRefresh = true;
    }
}

void DisplayBuffer::getColors(uint16_t* bg, uint16_t* fg)
{
    *bg = lookupColor(m_cursorColor & 0x0F);
    *fg = lookupColor((m_cursorColor) >> 4 & 0x0F);
}
