#ifndef PIN_H
#define PIN_H

#include <inttypes.h>
#include <avr/io.h>
#include "utils.h"
#include "config.h"

#if ! defined (__AVR_ATmega16__)
#warning Check PIN_OFFSET and DDR_OFFSET definitions for this MCU
#endif

#define PIN_OFFSET (-2)
#define DDR_OFFSET (-1)

typedef enum {
    PullNone,
    PullUp,
//    PullDown, //Unsupported
    OpenDrain, //Emulated
    _Output = 0x80
} PinMode;

class DigitalOut
{
public:
    explicit DigitalOut(avr_port &port, uint8_t pin) :
        pin_(_BV(pin)),
        port_(&port)
    {
        port_->DDR |= pin_;
    }

    void operator=(uint8_t value) const
    {
        if (value)
            port_->PORT |= pin_;
        else
            port_->PORT &= ~pin_;
    }

    force_inline operator uint8_t() const
    {
        return port_->PORT & pin_;
    }
protected:
    const uint8_t pin_;
    avr_port *port_;
};

class DigitalInOut
{
public:
    explicit DigitalInOut(avr_port &port, uint8_t pin) :
        pin_(_BV(pin)),
        mode_(PullUp),
        port_(&port)
    {
        port_->DDR &= ~pin_;
    }

    void operator=(uint8_t value) const;


    force_inline operator uint8_t() const
    {
        return port_->PIN & pin_;
    }

    force_inline void output() const
    {
        port_->DDR |= pin_;
        mode_ |= _Output;
    }

    force_inline void input() const
    {
        port_->DDR &= ~pin_;
        mode_ &= ~_Output;
    }

    force_inline void mode(PinMode pull) const
    {
        mode_ = pull;
        if (pull == PullUp || pull == OpenDrain)
        {
            input();
            port_->PORT |= pin_; //Enable pullup
        }
        if (pull == PullNone)
        {
            input();
            port_->PORT &= ~pin_; //Disable pullup
        }
    }

protected:
    const uint8_t pin_;
    mutable uint8_t mode_; //This is an ugly hack to allow constructors such as XY(DigitalInOut(p1))
    avr_port *port_;
};




#endif // PIN_H


