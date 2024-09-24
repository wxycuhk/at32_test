#ifndef _74HC164_H
#define _74HC164_H

#include "sys.h"
#include "at32f423.h"

# define HC164_CLK_HIGH  PCout(10) = 1
# define HC164_CLK_LOW   PCout(10) = 0

# define HC164_CLR  PBout(2)
# define HC164_DAT  PCout(12)

void hc164_init(void);
void ShiftOut(uint8_t data);

#endif
