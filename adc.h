#ifndef ADC_H
#define ADC_H

#include "utils.h"

class AnalogIn
{
public:
    AnalogIn(uint8_t channel) : channel_(channel/*| _BV(ADLAR)*/ |_BV(REFS0)) { }
    force_inline operator int16_t() {
        return read_s16();
    }

    int16_t read_s16()
    {
        ADMUX = channel_;
        /* ADC operation frequency: 50kHz - 200kHz */
        unsigned factor = MCLK / 200000;
        uint8_t divider = 0;
        if (factor < 2) {
            divider = 0;
        } else if (factor < 4)
        {
            divider = _BV(ADPS1);
        } else if (factor < 8)
        {
            divider = _BV(ADPS1) | _BV(ADPS0);
        } else if (factor < 16)
        {
            divider = _BV(ADPS2);
        } else if (factor < 32)
        {
            divider = _BV(ADPS2) | _BV(ADPS0);
        } else if (factor < 64)
        {
            divider = _BV(ADPS2) | _BV(ADPS1);
        } else if (factor < 128)
        {
            divider = _BV(ADPS2) | _BV(ADPS1) | _BV(ADPS0);
        }
        /* divider should be evaluated at compile time */
        ADCSRA = _BV(ADEN) | _BV(ADSC) | divider;
        while (ADCSRA & _BV(ADSC));
        return ADC;
    }

    force_inline void disable() {}
    force_inline void enable() {}
private:
    uint8_t channel_;
};

//int16_t AnalogIn::read_s16()
//{


//}

#endif // ADC_H



