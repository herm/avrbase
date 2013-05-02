#include "debug.h"
#include "uart.h"

#include <stdlib.h>

static Serial debug_uart(38400);

static void debug_hex(uint8_t x)
{
    uint8_t tmp = x >> 4;
    if (tmp <= 9) debug_uart.putc(tmp+'0'); else debug_uart.putc(tmp-10+'A');
    tmp = x & 0x0f;
    if (tmp <= 9) debug_uart.putc(tmp+'0'); else debug_uart.putc(tmp-10+'A');
}

static void debug_write(const uint8_t *val, uint8_t len, uint8_t bytes)
{
    for (uint8_t i=0; i < len; i++)
    {
        for (int8_t b=bytes - 1; b>=0; b--)
        {
            debug_hex(val[i+b]);
        }
        debug_uart.putc(' ');
    }
    debug_uart.newline();
}

void dbg_write_u32(const uint32_t *val, uint8_t len)
{
    debug_write((const uint8_t*)val, len, 4);
}

void dbg_write_u16(const uint16_t *val, uint8_t len)
{
    debug_write((const uint8_t*)val, len, 2);
}

void dbg_write_u8(const uint8_t *val, uint8_t len)
{
    debug_write(val, len, 1);
}

void dbg_write_str(const char *msg, bool newline)
{
    debug_uart.print(msg);
    if (newline) debug_uart.newline();
}

void dbg_write_char(char msg)
{
    debug_uart.putc(msg);
    debug_uart.newline();
}

void dbg_write_decimal(int32_t val)
{
    char buffer[12];
    ultoa(val, buffer, 10);
    debug_uart.print(buffer);
    debug_uart.newline();
}

void dbg_write_float(double f, bool newline)
{
    char buffer[20];
    dtostrf(f, 0, 2, buffer);
    debug_uart.print(buffer);
    if (newline) debug_uart.newline(); else debug_uart.putc(' ');
}
