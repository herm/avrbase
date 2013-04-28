#include "debug.h"
#include "uart.h"

static Serial debug_uart(38400);

static void debug_hex(uint8_t x)
{
    uint8_t tmp = x >> 4;
    if (tmp <= 9) debug_uart.putc(tmp+'0'); else debug_uart.putc(tmp-10+'A');
    tmp = x & 0x0f;
    if (tmp <= 9) debug_uart.putc(tmp+'0'); else debug_uart.putc(tmp-10+'A');
}

static void debug_write(const uint8_t *val, long len, uint8_t bytes)
{
    uint8_t count = 0;
    const uint8_t *end = val + len;
    while (val != end)
    {
        debug_hex(*val++);
        if (++count == bytes) {
            count = 0;
            debug_uart.putc(' ');
        }
    }
    debug_uart.newline();
}

void dbg_write_u32(const uint32_t *val, uint8_t len)
{
    debug_write((const uint8_t*)val, len*4, 4);
}

void dbg_write_u16(const uint16_t *val, uint8_t len)
{
    debug_write((const uint8_t*)val, len*2, 2);
}

void dbg_write_u8(const uint8_t *val, uint8_t len)
{
    debug_write(val, len, 1);
}

void dbg_write_str(const char *msg)
{
    debug_uart.print(msg);
    debug_uart.newline();
}

void dbg_write_char(char msg)
{
    debug_uart.putc(msg);
    debug_uart.newline();
}
