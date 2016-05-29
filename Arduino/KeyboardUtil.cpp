// 
// 
// 

#include <PS2KeyAdvanced.h>
#include "KeyboardUtil.h"

////////////////////////////////////////////////////////////////////////////////
// ASCII Characters
#define CR	'\r'
#define NL	'\n'
#define LF      0x0a
#define TAB	'\t'
#define BELL	'\b'
#define SPACE   ' '
#define SQUOTE  '\''
#define DQUOTE  '\"'
#define CTRLC	0x03
#define CTRLH	0x08
#define CTRLS	0x13
#define CTRLX	0x18
#define DELETE  0x7F

// Returns: true if key is printable, false if not
bool TranslateKey(uint16_t code, char *pOutChar)
{
	bool printable = false;

	// The top bits are status and the bottom bits are the scan code.
	// We need to check the status code, because we only care about keydown, not keyup
	char c = ((char)code & 0xFF);
	bool keydown = ((code & PS2_BREAK) == 0); // break flag means keyup

	if (c >= 0x20 && c <= 0x61) // regular printables
	{
		printable = true;
		if (c >= 0x41 && c <= 0x5A) // alpha
		{
			// TODO: handle shift key
		}

		*pOutChar = c; // TODO
	}
	else if (c == PS2_KEY_SPACE)
	{
		printable = true;
		*pOutChar = SPACE;
	}
	else if (c == PS2_KEY_BS) 
	{
		printable = true;
		*pOutChar = DELETE;
	}
	else if (c == PS2_KEY_ENTER)
	{
		printable = true;
		*pOutChar = NL;
	}

	return printable && keydown;
}
