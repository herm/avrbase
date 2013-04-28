#ifndef UART_H
#define UART_H

#include <avr/io.h>
#include "config.h"
#include "utils.h"


class Serial
{
public:
    Serial(uint32_t baudrate)
    {
        baud(baudrate);
    }

    force_inline void baud(uint32_t baudrate)
    {
        /* Rounding! */
        uint16_t ubbr_value = (MCLK+4l*baudrate)/(8l*baudrate) - 1;
        UBRRH = ubbr_value >> 8;
        UBRRL = ubbr_value;
        UCSRA = _BV(U2X);
        UCSRB = _BV(RXEN) | _BV(TXEN);
        UCSRC = _BV(URSEL) | _BV(UCSZ0) |_BV(UCSZ1);
    }

    void putc(uint8_t c)
    {
        while (!(UCSRA & _BV(UDRE)));
        UDR = c;
    }

    void print(const char *str)
    {
        while (*str) { putc(*str++); }
    }

    uint8_t getc()
    {
        while (!(UCSRA & _BV(RXC)));
        return UDR;
    }

    void newline()
    {
        putc('\r');
        putc('\n');
    }

};

#endif // UART_H
