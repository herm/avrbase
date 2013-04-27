#ifndef DELAY_H
#define DELAY_H

#include "config.h"
#include <util/delay.h>
#include <avr/cpufunc.h>

#define delay_us(x) _delay_us(x)
#define delay_ms(x) _delay_ms(x)
#define nop() _NOP()

#endif // DELAY_H
