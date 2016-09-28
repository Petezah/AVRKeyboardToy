// 
// AVR Keyboard Toy
// 
// Authors: Peter Dunshee <peter@petezah.com>
// 

#include <Arduino.h>
#include "AvrKeyboardToy.h"

#include <PS2KeyAdvanced.h>
#include "KeyboardUtil.h"
#include "TinyBasicPlus.h"

// Keyboard Pins
#define KEYBOARD_DATA 4
#define KEYBOARD_CLK 3

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

// Special, serial-only commands
#define CURS_UP CTRLW
#define CURS_LF CTRLA
#define CURS_DN CTRLS
#define CURS_RT CTRLD
//
    
Adafruit_ST7735 g_display(TFT_CS, TFT_DC, TFT_RST);
DisplayBuffer g_displayBuffer(&g_display);
PS2KeyAdvanced g_keyboard;

void displayTestPattern() 
{
    while(!g_displayBuffer.DisplayNeedsRefresh())  // fill until refresh is triggered
    {
      static char c = 0;
      g_displayBuffer.write(c++, true);
    }

    for(int i=0; i<7; ++i) g_displayBuffer.write((char)65+i, true);
    g_displayBuffer.GetBuffer()[0] = 65;
}

AvrKeyboardToy::AvrKeyboardToy() :
    m_displayBGcolor(ST7735_BLUE), m_displayFGcolor(ST7735_WHITE),
    m_cursorX(0), m_cursorY(0), m_cursorVisible(true), m_lastCursorMillis(0),
    m_keyboardIsActive(false)
{
}

void AvrKeyboardToy::Init()
{
    // Arduino init
    init();

    Serial.begin(9600);
    Serial.println(F("AvrKeyboardToy v"AVRKEYTOY_VERSION_STRING));

    // TODO: our own init
    InitDisplay();
    InitInput();

    //displayTestPattern();

    RefreshDisplay(false);
}

void AvrKeyboardToy::InitDisplay()
{
    g_display.initR(INITR_BLACKTAB);  // You will need to do this in every sketch
    g_display.setRotation(2); // 180* rotation

    // Power on the display
    pinMode(TFT_EN, OUTPUT);
    digitalWrite(TFT_EN, LOW); // TFT is enabled LOW
}

void AvrKeyboardToy::InitInput()
{
    g_keyboard.begin(KEYBOARD_DATA, KEYBOARD_CLK);
    g_keyboard.echo();  // ping keyboard to see if there
    delay( 6 );
    if( (g_keyboard.read() & 0xFF) == PS2_KEY_ECHO )
    {
        m_keyboardIsActive = true;
        Serial.println(F("Found keyboard!  Enabling keyboard for input"));
    }
    else
    {
        m_keyboardIsActive = false;
        Serial.println(F("No keyboard was found.  Enabling UART serial input"));
    } 
}

void AvrKeyboardToy::Update()
{
    UpdateInput();
	UpdateInterpreter();
    UpdateSerial();
    if(UpdateCursor())
    {
        RefreshDisplay(false);
    }
    //g_displayBuffer.scrollBufferUp();
}

void AvrKeyboardToy::UpdateInput()
{
    uint16_t code = 0;
    char c;
    bool gotChar = false;
    
    // Only try to read the keyboard if it is active
    if (m_keyboardIsActive)
    {
        if(g_keyboard.available())
        {
            code = g_keyboard.read();
            gotChar = TranslateKey(code, &c); // if it is printable, we will print it (gotChar)
            if(!gotChar) // if it is not printable, we may be able to do something anyway
            {
                DispatchSpecialKeyboardInput(c, code);
            }
        }
    }
    else if (Serial.available()) // Keep serial for debug purposes
    {
        c = Serial.read();
        gotChar = !DispatchSerialInput(c); // handle all printable chars below
    }

    if(gotChar)
    {
        DispatchInputChar(c, code);
    }
}

void AvrKeyboardToy::UpdateInterpreter()
{
    static bool setupBasic = true;
    if(setupBasic)
    {
        setupBasic = false;
        setupBASIC();
        performBASICWarmStart();
    }
}

void AvrKeyboardToy::UpdateSerial()
{
	if (serialEventRun) serialEventRun();
}

bool AvrKeyboardToy::UpdateCursor()
{
    int16_t bufX = g_displayBuffer.getCursorX();
    int16_t bufY = g_displayBuffer.getCursorY();
    char* pC = g_displayBuffer.GetBuffer() + m_cursorY * NUM_CHAR_COLUMNS + m_cursorX;
    if (bufX != m_cursorX || bufY != m_cursorY)
    {
        m_lastCursorMillis = millis();
        m_cursorVisible = true;
        g_display.drawFastChar(
            m_cursorX*CHAR_WIDTH, m_cursorY*CHAR_HEIGHT, *pC, 
            m_displayFGcolor,
            m_displayBGcolor
            );
        m_cursorX = bufX;
        m_cursorY = bufY;
    }

    unsigned long cursorMillis = millis();
    bool result = false;
	if ((cursorMillis - m_lastCursorMillis) > 1000)
	{
	  m_lastCursorMillis = cursorMillis;
	  m_cursorVisible = m_cursorVisible ? false : true;
      result = !m_cursorVisible;
	}
    pC = g_displayBuffer.GetBuffer() + m_cursorY * NUM_CHAR_COLUMNS + m_cursorX;
    g_display.drawFastChar(
        m_cursorX*CHAR_WIDTH, m_cursorY*CHAR_HEIGHT, *pC, 
        m_cursorVisible ? m_displayBGcolor : m_displayFGcolor,
        m_cursorVisible ? m_displayFGcolor : m_displayBGcolor
        );

    return result; // signal that a refresh can be done safely
}

void AvrKeyboardToy::RefreshDisplay(bool clearOnly)
{
    if (clearOnly) 
    {
        g_display.fillScreen(ST7735_BLUE);
        return;
    }

    char* pC = g_displayBuffer.GetBuffer();
    g_display.drawFastCharBuffer((unsigned char*)pC, ST7735_WHITE, ST7735_BLUE);
}

bool AvrKeyboardToy::DispatchSerialInput(char c) // returns true if handled
{
    switch (c)
    {
    case ESC: //serial escape sequence
    {
        const int maxNumEscChars = 5;
        char escChars[maxNumEscChars] = {0};
        int receivedEscChars = 0;
        while (Serial.available() && receivedEscChars < maxNumEscChars) 
            escChars[receivedEscChars++] = Serial.read();
        
        // Handle cursor keys
        if (escChars[0] == LBRACKET)
        {
            switch(escChars[1])
            {
            case 'A': g_displayBuffer.moveCursorUp(); break;
            case 'B': g_displayBuffer.moveCursorDown(); break;
            case 'C': g_displayBuffer.moveCursorRight(); break;
            case 'D': g_displayBuffer.moveCursorLeft(); break;
            default: 
                break; // do nothing
            }
        }
        break; // handle all escape codes
    }

    // Our own serial commands; n/a in keyboard mode!
    // This will make our life easier, since PuTTY does not
    // send esc sequences correctly for cursor at least
    case CURS_UP: g_displayBuffer.moveCursorUp(); break;
    case CURS_DN: g_displayBuffer.moveCursorDown(); break;
    case CURS_RT: g_displayBuffer.moveCursorRight(); break;
    case CURS_LF: g_displayBuffer.moveCursorLeft(); break;
        
    default:
        // printable characters; do not handle
        return false;
    }

    // handle everything that falls through above;
    // only printables we don't handle end up in "default"
    return true;
}

void AvrKeyboardToy::DispatchSpecialKeyboardInput(char c, uint16_t code)
{
    // The top bits are status and the bottom bits are the scan code.
	// We need to check the status code, because we only care about keydown, not keyup
	char scanCode = ((char)code & 0xFF);
	bool keydown = ((code & PS2_BREAK) == 0); // break flag means keyup
	bool shift = ((code & PS2_SHIFT) != 0);
	bool ctrl = ((code & PS2_CTRL) != 0);
	bool alt = ((code & PS2_ALT) != 0);

    if (keydown)
    {
        switch (scanCode)
        {
        case PS2_KEY_UP_ARROW: g_displayBuffer.moveCursorUp(); break;
        case PS2_KEY_DN_ARROW: g_displayBuffer.moveCursorDown(); break;
        case PS2_KEY_R_ARROW: g_displayBuffer.moveCursorRight(); break;
        case PS2_KEY_L_ARROW: g_displayBuffer.moveCursorLeft(); break;
        
        default:
            break;
        }
    }
}

void AvrKeyboardToy::DispatchInputChar(char c, uint16_t code)
{
    switch(c)
    {
        case NL:
        case CR:
            PerformLineTermination();
            return;

        default:
            //Serial.print(":"); Serial.print((int)c); Serial.print(";");
            OutputChar(c);
            return;
    }
}

void AvrKeyboardToy::OutputChar(char c)
{
    Serial.write(c); // echo on serial
    g_displayBuffer.write(c);
}

void AvrKeyboardToy::PerformLineTermination()
{
    // Read the line under the cursor
    char execLine[NUM_CHAR_COLUMNS + 1] = {0};
    char *pDispLine = g_displayBuffer.GetCursorLine();
    memcpy(execLine, pDispLine, NUM_CHAR_COLUMNS);

    // Output line terminators
    OutputChar(NL);
    OutputChar(CR);

    // Execute line
    Serial.print(F("Executing: "));
    Serial.println(execLine);
    
    injectln(execLine);
    BASICRunState state = execBASIC();
    if (state == WarmStart)
    {
        performBASICWarmStart();
    }
}
