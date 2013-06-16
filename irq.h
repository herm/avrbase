#ifndef IRQ_H
#define IRQ_H

#include <avr/interrupt.h>
#include "utils.h"
typedef void (*irq_func_t)();

force_inline static inline void enableIRQ()
{
    sei();
}

force_inline static inline void disableIRQ()
{
    cli();
}

#endif // IRQ_H
