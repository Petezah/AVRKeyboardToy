// KeyboardUtil.h

#ifndef _KEYBOARDUTIL_h
#define _KEYBOARDUTIL_h

#include <stdint.h>

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
#define CTRLA 0x01
#define CTRLC	0x03
#define CTRLD 0x04
#define CTRLH	0x08
#define CTRLI   0x09
#define CTRLJ   0x0A
#define CTRLK   0x0B
#define CTRLL	0x0C
#define CTRLS	0x13
#define CTRLW 0x17
#define CTRLX	0x18
#define ESC     0x1B
#define LBRACKET '['
#define DELETE  0x7F

bool TranslateKey(uint16_t code, char *pOutChar);

#endif

