#ifndef DEBUG_H
#define DEBUG_H

#include <stdint.h>

void dbg_write_u32(const uint32_t *val, uint8_t len);
void dbg_write_u16(const uint16_t *val, uint8_t len);
void dbg_write_u8(const uint8_t *val, uint8_t len);

void dbg_write_str(const char *msg, bool newline=true);
void dbg_write_char(char msg);
void dbg_write_decimal(int32_t val);
void dbg_write_float(double f);


#endif // DEBUG_H
