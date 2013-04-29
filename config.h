#ifndef _CONFIG_H_
#define _CONFIG_H_
#include "config.avrbase.h"

typedef volatile unsigned char  AVR_REG8  __attribute__ (( aligned (1)));

typedef struct
{
    AVR_REG8  PIN;
    AVR_REG8  DDR;
    AVR_REG8  PORT;
} __attribute__ (( packed )) avr_port;

#define avr_port_cast(port) (*(const_cast<avr_port *>((volatile avr_port *)&port)))

#define avr_portA   avr_port_cast(PINA)
#define avr_portB   avr_port_cast(PINB)
#define avr_portC   avr_port_cast(PINC)
#define avr_portD   avr_port_cast(PIND)

#define F_CPU MCLK
#endif
