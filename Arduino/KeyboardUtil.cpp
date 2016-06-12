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
	bool shift = ((code & PS2_SHIFT) != 0);
	bool ctrl = ((code & PS2_CTRL) != 0);
	bool alt = ((code & PS2_ALT) != 0);

	if (c >= 0x20 && c <= 0x60) // regular printables
	{
		printable = true;
		if (c >= PS2_KEY_A && c <= PS2_KEY_Z) // alpha
		{
			if (!shift) // c will, by default, be shifted alpha; so handle non-shifted
			{
				char alpha = c - PS2_KEY_A;
				*pOutChar = 'a' + alpha;
			}
			else
			{
				*pOutChar = c;
			}
		}
		else if (c >= PS2_KEY_0 && c <= PS2_KEY_9 && shift)
		{
			char *pSpecial = ")!@#$%^&*(";
			char idx = c - PS2_KEY_0;
			*pOutChar = pSpecial[idx];
		}
		else if (c >= PS2_KEY_KP0 && c <= PS2_KEY_KP9)
		{
			char num = c - PS2_KEY_KP0;
			*pOutChar = '0' + num;
		}
		else
		{
			switch (c)
			{
			case PS2_KEY_KP_DOT   : *pOutChar = '.';  break; //0x2A
			case PS2_KEY_KP_ENTER : *pOutChar = '\n'; break; //0x2B
			case PS2_KEY_KP_PLUS  : *pOutChar = '+';  break; //0x2C
			case PS2_KEY_KP_MINUS : *pOutChar = '-';  break; //0x2D
			case PS2_KEY_KP_TIMES : *pOutChar = '*';  break; //0x2E
			case PS2_KEY_KP_DIV   : *pOutChar = '/';  break; //0x2F
			case PS2_KEY_APOS     : *pOutChar = shift ? '\"' : '\''; break; //0X3A
			case PS2_KEY_COMMA    : *pOutChar = shift ? '<' : ',';  break; //0X3B
			case PS2_KEY_MINUS    : *pOutChar = shift ? '_' : '-';  break; //0X3C
			case PS2_KEY_DOT      : *pOutChar = shift ? '>' : '.';  break; //0X3D
			case PS2_KEY_DIV      : *pOutChar = shift ? '?' : '/';  break; //0X3E
			/* Some Numeric keyboards have an '=' on right keypad */
			case PS2_KEY_KP_EQUAL : *pOutChar = '=';  break; //0x3F
			/* Single quote or back quote */
			case PS2_KEY_SINGLE   : *pOutChar = shift ? '~' : '`';  break; //0X40
			case PS2_KEY_SEMI     : *pOutChar = shift ? ':' : ';';  break; //0X5B
			case PS2_KEY_BACK     : *pOutChar = shift ? '|' : '\\'; break; //0X5C
			case PS2_KEY_OPEN_SQ  : *pOutChar = shift ? '{' : '[';  break; //0X5D
			case PS2_KEY_CLOSE_SQ : *pOutChar = shift ? '}' : ']';  break; //0X5E
			case PS2_KEY_EQUAL    : *pOutChar = shift ? '+' : '=';  break; //0X5F
			/* Some Numeric keypads have a comma key */
			case PS2_KEY_KP_COMMA : *pOutChar = ',';  break; //0x60
			default:
				*pOutChar = c; // TODO?
			}
		}
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
