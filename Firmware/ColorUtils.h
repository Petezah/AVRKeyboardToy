
#ifndef _COLORUTILS_H_
#define _COLORUTILS_H_

#include <stdint.h>

uint16_t color565(uint8_t r, uint8_t g, uint8_t b);
uint16_t lookupColor(uint8_t index);
uint16_t lookupLerpColor(uint8_t index1, uint8_t index2);

#endif
