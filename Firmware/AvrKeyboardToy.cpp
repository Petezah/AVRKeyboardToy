//
// AVR Keyboard Toy
//
// Authors: Peter Dunshee <peter@petezah.com>
//

#include <Arduino.h>
#include "AvrKeyboardToy.h"

//#define PS2_KEYBOARD
#define SRXE_KEYBOARD

#include <PS2KeyAdvanced.h>
#ifdef PS2_KEYBOARD
// Keyboard Pins
#define KEYBOARD_DATA 4
#define KEYBOARD_CLK 3
#endif


#include "KeyboardUtil.h"


#if defined(AVR_KEYBOARD_TOY_RELEASE)

// Display utilities
#define TFT_CS 10
#define TFT_RST 17 // PC3=D17=A3
#define TFT_DC 8

#define TFT_EN 14

#elif defined(AVR_KEYBOARD_TOY_TEST)

// Display utilities
#define TFT_CS 17 // PC3=D17=A3
#define TFT_RST 2 // PD2
#define TFT_DC 8

#define TFT_EN 14

#else
#error No board type was defined!  Must define either AVR_KEYBOARD_TOY_RELEASE or AVR_KEYBOARD_TOY_TEST!
#endif

//TEMP: When I get SD cooperating with TFT, I will move this
#define TFT_SD_EN 15

#define TFT_SCLK 13
#define TFT_MOSI 11
////

// NB: Tone speaker uses D9/PB1

// Special, serial-only commands
#define CURS_UP CTRLW
#define CURS_LF CTRLA
#define CURS_DN CTRLS
#define CURS_RT CTRLD
//

// String table
static const char VERSION_STRING[] PROGMEM = "AvrKeyboardToy v" AVRKEYTOY_VERSION_STRING;
static const char FOUND_KEYBOARD[] PROGMEM = "Found keyboard!  Enabling keyboard for input";
static const char NO_KEYBOARD_FOUND[] PROGMEM = "No keyboard was found.  Enabling UART serial input";
static const char EXECUTING_PROMPT[] PROGMEM = "Executing: ";
///////////////

SRXE_Display g_display;
DisplayBuffer g_displayBuffer(&g_display);
#ifdef PS2_KEYBOARD
PS2KeyAdvanced g_keyboard;
#endif

void displayTestPattern()
{
    while (!g_displayBuffer.DisplayNeedsRefresh()) // fill until refresh is triggered
    {
        static char c = 0;
        g_displayBuffer.write(c++, true);
    }

    for (int i = 0; i < 7; ++i)
        g_displayBuffer.write((char)65 + i, true);
    g_displayBuffer.GetBuffer()[0] = 65;
}

void simpleDisplayTest()
{
    g_display.fillScreen(0); // BLACK?
    g_display.println("This is a test");
    g_display.println("This is another test");
    while (1)
        ;
}

void simpleDisplayTest2()
{
    SRXEFill(0);
    SRXEWriteString(0, 120, (char*)"Hello World!", FONT_LARGE, 3, 0);
    SRXEWriteString(0, 90, (char*)"Hello World!", FONT_SMALL, 3, 0);
    SRXEWriteString(0, 100, (char*)"Hello World!", FONT_MEDIUM, 3, 0);
    SRXESetPosition(0, 30, 12, 16);

    uint8_t data[64] = {
        0, 0, 0, 0,
        0, 0, 0, 0,
        0, 0, 0, 0,
        3, 3, 3, 3,
        3, 3, 3, 3,
        3, 3, 3, 3,
        0x1c, 0x1c, 0x1c, 0x1c,
        0x1c, 0x1c, 0x1c, 0x1c,
        0x1c, 0x1c, 0x1c, 0x1c,
        0xe0, 0xe0, 0xe0, 0xe0,
        0xe0, 0xe0, 0xe0, 0xe0,
        0xe0, 0xe0, 0xe0, 0xe0,
        1, 1, 1, 1, 
        2, 2, 2, 2, 
        3, 3, 3, 3, 
        3, 3,  3, 3 
    };
    SRXEWriteDataBlock(data, 64);
    SRXERectangle(0, 0, 20, 20, 3, 1);
    SRXERectangle(0, 20, 20, 20, 2, 1);
    SRXERectangle(0, 40, 20, 20, 1, 1);
    while (1)
        ;
}

AvrKeyboardToy::AvrKeyboardToy() : m_cursorX(0), m_cursorY(0), m_cursorVisible(true), m_lastCursorMillis(0), m_displayNeedsRefresh(false),
                                   m_keyboardIsActive(false),
                                   m_interpreterState(InitStart)
{
}

void AvrKeyboardToy::Init()
{
    // Arduino init
    init();

    Serial.begin(9600);
    OutputString(VERSION_STRING, true);

    // TODO: our own init
    InitDisplay();
    InitInput();

    //displayTestPattern();
    //simpleDisplayTest();
    //simpleDisplayTest2();

    m_displayNeedsRefresh = true;
}

void AvrKeyboardToy::InitDisplay()
{
    g_display.init();
    //g_display.setRotation(2); // 180* rotation

    // Power on the display
    //pinMode(TFT_EN, OUTPUT);
    //digitalWrite(TFT_EN, LOW); // TFT is enabled LOW
    m_displayEnabled = true;
}

void AvrKeyboardToy::InitInput()
{
#ifdef PS2_KEYBOARD
    g_keyboard.begin(KEYBOARD_DATA);
    g_keyboard.echo(); // ping keyboard to see if there
    delay(6);
    if ((g_keyboard.read() & 0xFF) == PS2_KEY_ECHO)
    {
        m_keyboardIsActive = true;
        OutputString(FOUND_KEYBOARD, true);

        delay(10);
        g_keyboard.resetKey();
    }
    else
    {
        m_keyboardIsActive = false;
        OutputString(NO_KEYBOARD_FOUND, true);
    }
#endif
#ifdef SRXE_KEYBOARD
    m_keyboardIsActive = true;
#endif

    m_lastInputMillis = millis();
}

void AvrKeyboardToy::Update()
{
    unsigned long currentMillis = millis();
    unsigned long deltaMillis = currentMillis - m_lastInputMillis;
    // if (deltaMillis > 5000)
    // {
    //     Serial.println("Long input delay; putting display to sleep...");
    //     m_displayEnabled = false;
    //     digitalWrite(TFT_EN, HIGH); // TFT is enabled LOW
    // }

    if (m_displayEnabled)
    {
        UpdateInterpreter();
        UpdateInput();
        UpdateCursor();
        UpdateSerial();

        if (m_displayNeedsRefresh)
        {
            m_displayNeedsRefresh = false;
            g_displayBuffer.RefreshDisplay();
            //RefreshDisplay(false);
        }
        //g_displayBuffer.scrollBufferUp();
    }
    else
    {
        UpdateInput();
        UpdateSerial();
    }
}

void AvrKeyboardToy::UpdateInput()
{
    uint16_t code = 0;
    char c;
    bool gotChar = false;

    // Only try to read the keyboard if it is active
    if (m_keyboardIsActive)
    {
        //Serial.print('.');
#ifdef PS2_KEYBOARD
        if (g_keyboard.available())
        {
            code = g_keyboard.read();
            //Serial.print("Got code ");
            //Serial.println(code);
            gotChar = TranslateKey(code, &c); // if it is printable, we will print it (gotChar)
            if (!gotChar)                     // if it is not printable, we may be able to do something anyway
            {
                DispatchSpecialKeyboardInput(c, code);
            }
        }
#endif
#ifdef SRXE_KEYBOARD
        byte code = SRXEGetKey();
        gotChar = TranslateKey(code, &c); // if it is printable, we will print it (gotChar)
        if (!gotChar)                     // if it is not printable, we may be able to do something anyway
        {
            DispatchSpecialKeyboardInput(c, code);
        }
#endif
    }
#ifndef SRXE_KEYBOARD
    else if (Serial.available()) // Keep serial for debug purposes
    {
        //Serial.print(',');
        c = Serial.read();
        gotChar = !DispatchSerialInput(c); // handle all printable chars below
    }
#endif

    if (gotChar)
    {
        DispatchInputChar(c, code);
    }
}

void AvrKeyboardToy::UpdateInterpreter()
{
    //Serial.print((int)m_interpreterState);
    switch (m_interpreterState)
    {
    case InitStart:
        m_interpreterState = WarmStart;
        setupBASIC();
        break;

    case WarmStart:
        m_interpreterState = Idle;
        performBASICWarmStart();
        break;

    case Run:
        m_interpreterState = execBASIC();
        break;

    default:
        break;
    }
}

void AvrKeyboardToy::UpdateSerial()
{
    if (serialEventRun)
        serialEventRun();
}

bool AvrKeyboardToy::UpdateCursor()
{
    uint16_t bg, fg;
    bool result = false;
    g_displayBuffer.getColors(&bg, &fg);

    int16_t bufX = g_displayBuffer.getCursorX();
    int16_t bufY = g_displayBuffer.getCursorY();
    char *pC = g_displayBuffer.GetBuffer() + m_cursorY * NUM_CHAR_COLUMNS + m_cursorX;
    if (bufX != m_cursorX || bufY != m_cursorY)
    {
        m_lastCursorMillis = millis();
        m_cursorVisible = true;
        g_display.drawFastChar(
            m_cursorX * CHAR_WIDTH, m_cursorY * CHAR_HEIGHT, *pC,
            fg,
            bg);
        m_cursorX = bufX;
        m_cursorY = bufY;
    }

    unsigned long cursorMillis = millis();
    if ((cursorMillis - m_lastCursorMillis) > 1000)
    {
        m_lastCursorMillis = cursorMillis;
        m_cursorVisible = m_cursorVisible ? false : true;
        //result = !m_cursorVisible; // TODO: this is not the right place to signal a refresh
    }
    pC = g_displayBuffer.GetBuffer() + m_cursorY * NUM_CHAR_COLUMNS + m_cursorX;
    g_display.drawFastChar(
        m_cursorX * CHAR_WIDTH, m_cursorY * CHAR_HEIGHT, *pC,
        m_cursorVisible ? bg : fg,
        m_cursorVisible ? fg : bg);

    return result; // signal that a refresh can be done safely
}

void AvrKeyboardToy::RefreshDisplay(bool clearOnly)
{
    if (clearOnly)
    {
        g_display.fillScreen(0x001F);
        return;
    }

    char *pC = g_displayBuffer.GetBuffer();
    uint8_t *pColorBuf = g_displayBuffer.GetColorBuffer();
    g_display.drawFastCharBuffer((unsigned char *)pC, pColorBuf);
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
            switch (escChars[1])
            {
            case 'A':
                g_displayBuffer.moveCursorUp();
                break;
            case 'B':
                g_displayBuffer.moveCursorDown();
                break;
            case 'C':
                g_displayBuffer.moveCursorRight();
                break;
            case 'D':
                g_displayBuffer.moveCursorLeft();
                break;
            default:
                break; // do nothing
            }
        }
        break; // handle all escape codes
    }

    // Our own serial commands; n/a in keyboard mode!
    // This will make our life easier, since PuTTY does not
    // send esc sequences correctly for cursor at least
    case CURS_UP:
        g_displayBuffer.moveCursorUp();
        break;
    case CURS_DN:
        g_displayBuffer.moveCursorDown();
        break;
    case CURS_RT:
        g_displayBuffer.moveCursorRight();
        break;
    case CURS_LF:
        g_displayBuffer.moveCursorLeft();
        break;

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
#ifdef PS2_KEYBOARD
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
        case PS2_KEY_UP_ARROW:
            g_displayBuffer.moveCursorUp();
            break;
        case PS2_KEY_DN_ARROW:
            g_displayBuffer.moveCursorDown();
            break;
        case PS2_KEY_R_ARROW:
            g_displayBuffer.moveCursorRight();
            break;
        case PS2_KEY_L_ARROW:
            g_displayBuffer.moveCursorLeft();
            break;

        default:
            if (scanCode >= PS2_KEY_F1 && scanCode <= PS2_KEY_F12)
            {
                DispatchFunctionKeyInput(shift, ctrl, alt, scanCode);
            }
            break;
        }
    }
#endif
#ifdef SRXE_KEYBOARD
    switch (c)
    {
    case 0x04:
        g_displayBuffer.moveCursorUp();
        break;
    case 0x05:
        g_displayBuffer.moveCursorDown();
        break;
    case 0x03:
        g_displayBuffer.moveCursorRight();
        break;
    case 0x02:
        g_displayBuffer.moveCursorLeft();
        break;

    default:
        // if (scanCode >= PS2_KEY_F1 && scanCode <= PS2_KEY_F12)
        // {
        //     DispatchFunctionKeyInput(shift, ctrl, alt, scanCode);
        // }
        break;
    }
#endif
}

void AvrKeyboardToy::DispatchFunctionKeyInput(bool shift, bool ctrl, bool alt, char scanCode)
{
    if (shift && !alt && !ctrl)
    {
        unsigned int freq = 300 * (scanCode - PS2_KEY_F1 + 1);
        tone(/*kPiezoPin*/ 9, freq, 200);
    }
    else if (shift && alt && !ctrl)
    {
        short int idx = (scanCode - PS2_KEY_F1) % 8;
        g_displayBuffer.setBgColor(idx);
    }
    else if (shift && !alt && ctrl)
    {
        short int idx = (scanCode - PS2_KEY_F1) % 8;
        g_displayBuffer.setBgColor(idx + 8);
    }
    else if (!shift && alt && !ctrl)
    {
        short int idx = (scanCode - PS2_KEY_F1) % 8;
        g_displayBuffer.setFgColor(idx);
    }
    else if (!shift && !alt && ctrl)
    {
        short int idx = (scanCode - PS2_KEY_F1) % 8;
        g_displayBuffer.setFgColor(idx + 8);
    }
}

void AvrKeyboardToy::DispatchInputChar(char c, uint16_t code)
{
    switch (c)
    {
    case NL:
    case CR:
        PerformLineTermination();
        return;

    case CTRLH:
    case DELETE:
        Serial.write(c); // echo on serial
        g_displayBuffer.backspace();
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
    char execLine[NUM_CHAR_COLUMNS + 2] = {0};
    char *pDispLine = g_displayBuffer.GetCursorLine();
    memcpy(execLine, pDispLine, NUM_CHAR_COLUMNS);
    execLine[NUM_CHAR_COLUMNS + 1] = 0;
    execLine[NUM_CHAR_COLUMNS] = ' ';

    // Find the last non-space char and terminate the command there
    for (int i = NUM_CHAR_COLUMNS; i >= 0; --i)
    {
        if (execLine[i] != ' ')
        {
            execLine[i + 1] = '\n';
            break;
        }
    }

    // Find the first non-space char and execute from there
    char *pExecCmd = execLine;
    while (*pExecCmd == ' ' && *pExecCmd != '\n') // also stop at our newline
    {
        ++pExecCmd;
    }

    // Output line terminators
    OutputChar(NL);
    OutputChar(CR);

    // Execute line
    OutputString(EXECUTING_PROMPT);
    Serial.println(pExecCmd);

    injectln(pExecCmd);
    m_interpreterState = Run;
}

void AvrKeyboardToy::OutputString(const char *msg, bool newline)
{
    while (1)
    {
        unsigned char c = pgm_read_byte(msg++);
        if (c == 0)
            break;
        if (!Serial.write(c))
            break;
    }
}
