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
    
Adafruit_ST7735 m_display(TFT_CS, TFT_DC, TFT_RST);
DisplayBuffer m_displayBuffer(&m_display);

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
    m_display.initR(INITR_BLACKTAB);  // You will need to do this in every sketch
    m_display.setRotation(2); // 180* rotation

    // Power on the display
    pinMode(TFT_EN, OUTPUT);
    digitalWrite(TFT_EN, LOW); // TFT is enabled LOW

    // TEST
    while(!m_displayBuffer.DisplayNeedsRefresh())// fill until refresh is triggered
    {
        static char c = 0;
        m_displayBuffer.write(c++, true);
    }

    RefreshDisplay(false);
}

void AvrKeyboardToy::Update()
{
	UpdateInterpreter();
    UpdateSerial();
    UpdateCursor();
    //RefreshDisplay(false);
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
    char* pC = m_displayBuffer.GetBuffer() + m_cursorY * NUM_CHAR_COLUMNS + m_cursorX;
    m_display.drawFastChar(
        m_cursorX*CHAR_WIDTH, m_cursorY*CHAR_HEIGHT, *pC, 
        m_cursorVisible ? m_displayBGcolor : m_displayFGcolor,
        m_cursorVisible ? m_displayFGcolor : m_displayBGcolor
        );
}

void AvrKeyboardToy::RefreshDisplay(bool clearOnly)
{
    //m_display.fillScreen(ST7735_BLUE);
    if (clearOnly) return;

    char* pC = m_displayBuffer.GetBuffer();
    unsigned int yoffset = 0;
    for (unsigned int y=0; y<NUM_CHAR_ROWS; ++y, yoffset += CHAR_HEIGHT)
    {
        unsigned int xoffset = 0;
        for (unsigned int x=0; x<NUM_CHAR_COLUMNS; ++x, ++pC, xoffset += CHAR_WIDTH)
        {
            //m_display.setCursor(xoffset, yoffset);
            //m_display.print(*pC);
            m_display.drawFastChar(xoffset, yoffset, *pC, ST7735_WHITE, ST7735_BLUE);
        }
    }
}