#ifndef SPI_HPP
#define SPI_HPP
#include <inttypes.h>
#include "utils.h"
#include <avr/io.h>
#include "config.h"

#if defined (__AVR_ATmega16__)
#define spi_DDR   DDRB
#define spi_SS    PB4
#define spi_MOSI  PB5
#define spi_MISO  PB6
#define spi_SCK   PB7
#else
#warning "Unknown MCU"
#endif

class SPI
{
public:
    SPI()
    {
        spi_DDR = _BV(spi_MOSI) | _BV(spi_SCK) | _BV(spi_SS);
        format();
        frequency();
    }
    inline void format(uint_fast8_t bits = 8, uint_fast8_t mode = 0)
    {
//        static_assert(bits == 10);
        if (bits != 8) {
            //TODO
        }
        if (mode != 0)
        {
            //TODO
        }
        SPCR = _BV(SPE) | _BV(MSTR);
    }

    force_inline void frequency(uint32_t freq = 1000000)
    {
        uint16_t divider = MCLK/freq;
        uint8_t SPSR_ = 0;
        uint8_t SPCR_ = 0;
        if (divider < 2)
        {
            SPSR_ = _BV(SPI2X);
        } else if (divider < 4) {

        } else if (divider < 8) {
            SPSR_ = _BV(SPI2X);
            SPCR_ = SPR0;
        } else if (divider < 16) {
            SPCR_ = SPR0;
        } else if (divider < 32) {
            SPSR_ = _BV(SPI2X);
            SPCR_ = _BV(SPR1);
        } else if (divider < 64) {
            SPCR_ = _BV(SPR1);
        } else if (divider < 128) {
            SPCR_ = _BV(SPR1 | SPR0);
        }
        SPSR = SPSR_;
        SPCR = (SPCR & ~(_BV(SPR1)|_BV(SPR0))) | SPCR_;
    }

    /* Status is shifted out while command is shifted in */
    uint_fast8_t write(uint_fast8_t data)
    {
        SPDR = data;
        while (!(SPSR & _BV(SPIF)));
        return SPDR;
    }
};
#endif // SPI_HPP
