#ifndef IRQ_H
#define IRQ_H

#include <avr/interrupt.h>

inline void enableIRQ()
{
    sei();
}

#endif // IRQ_H
