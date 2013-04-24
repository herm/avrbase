#ifndef UART_H
#define UART_H

#include <avr/io.h>
#include "utils.h"

#ifndef DEFAULT_BAUD
#define DEFAULT_BAUD 9600
#endif

class Serial
{
public:
    Serial(int baudrate)
    {
        if (baudrate) baud(baudrate);
    }

    force_inline void baud(int baudrate)
    {
        /* Rounding! */
        uint16_t ubbr_value = (MCLK+baudrate*8)/16/baudrate - 1;
        UBRRH = ubbr_value >> 8;
        UBRRL = ubbr_value;
        UCSRB = _BV(RXEN) | _BV(TXEN);
        UCSRC = _BV(URSEL) | _BV(UCSZ0) |_BV(UCSZ1);
    }

    void putc(uint8_t c)
    {
        while (!(UCSRA & _BV(UDRE)));
        UDR = c;
    }

    void print(uint8_t *str)
    {
        while (str) { putc(*str++); }
    }

    uint8_t getc()
    {
        while (!(UCSRA & _BV(RXC)));
        return UDR;
    }

};

#endif // UART_H
