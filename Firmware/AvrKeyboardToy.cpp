// 
// AVR Keyboard Toy
// 
// Authors: Peter Dunshee <peter@petezah.com>
// 

#include <Arduino.h>
#include "AvrKeyboardToy.h"

// Display utilities
#define TFT_CS     10
#define TFT_RST    9  
#define TFT_DC     8

#define TFT_EN     14

//TEMP: When I get SD cooperating with TFT, I will move this
#define TFT_SD_EN  15

#define TFT_SCLK 13   
#define TFT_MOSI 11   
////
    
char m_displayBuffer[DISPLAY_BUF_SIZE];
Adafruit_ST7735 m_display(TFT_CS, TFT_DC, TFT_RST);

AvrKeyboardToy::AvrKeyboardToy() :
    m_displayBGcolor(ST7735_BLUE), m_displayFGcolor(ST7735_WHITE),
    m_cursorX(0), m_cursorY(0), m_cursorVisible(true), m_lastCursorMillis(0)
{

}

void AvrKeyboardToy::Init()
{
    // Arduino init
    init();

    // TODO: our own init
    memset(m_displayBuffer, ' ', sizeof(m_displayBuffer));
	m_display.initR(INITR_BLACKTAB);  // You will need to do this in every sketch
    RefreshDisplay(true);
}

void AvrKeyboardToy::Update()
{
	UpdateInterpreter();
    UpdateSerial();
    UpdateCursor();
    //RefreshDisplay();

    // Test
    // static char c = 0x20;
    // *m_displayBuffer = c++;
    // char *pC = m_displayBuffer;
    // for (unsigned int i = 0; i<DISPLAY_BUF_SIZE; ++i, ++pC)
    // {
    //     //m_display.setCursor(0, 0);
    //     m_display.print(c++);

    //     *pC = c++;
    //     if(c > 0x7E) c = 0x20;
    // }
    // RefreshDisplay();
}

void AvrKeyboardToy::UpdateInterpreter()
{

}

void AvrKeyboardToy::UpdateSerial()
{
	if (serialEventRun) serialEventRun();
}

void AvrKeyboardToy::UpdateCursor()
{
    unsigned long cursorMillis = millis();
	if ((cursorMillis - m_lastCursorMillis) > 1000)
	{
	  m_lastCursorMillis = cursorMillis;
	  m_cursorVisible = m_cursorVisible ? false : true;
	}
	m_display.fillRect(m_cursorX, m_cursorY, CHAR_WIDTH, CHAR_HEIGHT, m_cursorVisible ? m_displayFGcolor : m_displayBGcolor);
}

void AvrKeyboardToy::RefreshDisplay(bool clearOnly)
{
    m_display.fillScreen(ST7735_BLUE);
    if (clearOnly) return;

    char* pC = m_displayBuffer;
    unsigned int yoffset = 0;
    for (unsigned int y=0; y<NUM_CHAR_ROWS; ++y, yoffset += CHAR_HEIGHT)
    {
        unsigned int xoffset = 0;
        for (unsigned int x=0; x<NUM_CHAR_COLUMNS; ++x, ++pC, xoffset += CHAR_WIDTH)
        {
            m_display.setCursor(xoffset, yoffset);
            m_display.print(*pC);
        }
    }
}