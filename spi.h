#ifndef SPI_HPP
#define SPI_HPP
#include <inttypes.h>
#include "utils.h"
#include <avr/io.h>
#include "config.h"

class SPI
{
public:
    SPI()
    {
        DDRspi = _BV(mosi) | _BV(sck);
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
