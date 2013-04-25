#ifndef PIN_H
#define PIN_H

#include <inttypes.h>
#include <avr/io.h>
#include "utils.h"

class DigitalOut
{
public:
    explicit DigitalOut(volatile uint8_t *port, uint8_t pin, bool init=false) :
        pin_(_BV(pin)),
        port_(port)
    { if (init) *(port_ - 1) |= _BV(pin); }
    force_inline void operator=(int value) const { if (value) *port_ |= pin_; else *port_ &= ~pin_; }
    force_inline operator int() const { return *port_ & pin_; }
private:
    const uint8_t pin_;
    volatile uint8_t * const port_;
};

#endif // PIN_H
