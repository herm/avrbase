#include "pin.h"

void DigitalInOut::operator =(uint8_t value) const
{
    if (mode_ & _Output)
    {
        if (value)
            port_->PORT |= pin_;
        else
            port_->PORT &= ~pin_;
    } else if (mode_ == OpenDrain)
    {
        if (value)
        {
            // 1 = pullup, input
            port_->DDR &= ~pin_; //input
            port_->PORT |= pin_; //pullup
        } else {
            // 0 = output, 0
            port_->PORT &= ~pin_;
            port_->DDR |= pin_;
        }
    }
}
