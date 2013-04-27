#include "pin.h"
void DigitalInOut::operator =(uint8_t value) const
{
    if (mode_ & _Output)
    {
        if (value)
            *port_ |= pin_;
        else
            *port_ &= ~pin_;
    } else if (mode_ == OpenDrain)
    {
        if (value)
        {
            // 1 = pullup, input
            *(port_ + DDR_OFFSET) &= ~pin_; //input
            *port_ = 1; //pullup
        } else {
            // 0 = output, 0
            *port_ = 0;
            *(port_ + DDR_OFFSET) |= pin_;
        }
    }
}
