
#include "DisplayBuffer.h"

DisplayBuffer::DisplayBuffer(IDisplay *pDisplay) :
    m_pOutputDisplay(pDisplay), m_cursor_x(0), m_cursor_y(0), m_displayNeedsRefresh(false)
{
    memset(m_displayBuffer, ' ', sizeof(m_displayBuffer));
}

// TODO
uint16_t textcolor = ST7735_WHITE;
uint16_t textbgcolor = ST7735_BLUE;

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
        setChar(m_cursor_x, m_cursor_y, c, textcolor, textbgcolor);
        m_cursor_x ++;
        resolveCursor();
    }
}

void DisplayBuffer::setChar(int16_t x, int16_t y, unsigned char c, uint16_t color, uint16_t bg)
{
    m_displayBuffer[y*NUM_CHAR_COLUMNS + x] = c;
    if(m_pOutputDisplay != NULL)
    {
        m_pOutputDisplay->drawFastChar(x*CHAR_WIDTH, y*CHAR_HEIGHT, c, color, bg);
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

    m_displayNeedsRefresh = true;
}
