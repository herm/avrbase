#ifndef PIN_H
#define PIN_H

#include <inttypes.h>
#include <avr/io.h>
#include "utils.h"

class DigitalOut
{
public:
    explicit DigitalOut(volatile uint8_t *port, uint8_t pin, bool init_=false) :
        pin_(_BV(pin)),
        port_(port)
    { if (init_) *(port_ - 1) |= _BV(pin); }
//    void low() const { FIOCLR = bitmask_;}
//    void high() const { FIOSET = bitmask_;}
    force_inline void operator=(int value) const { if (value) *port_ |= pin_; else *port_ &= ~pin_; }
//    force_inline operator int() const { return FIOPIN & bitmask_ ? 1 : 0; }
private:
    const uint8_t pin_;
    volatile uint8_t * const port_;
};

#endif // PIN_H
