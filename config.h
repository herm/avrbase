#include "config.avrbase.h"

#if defined (__AVR_ATmega8__)
/* SPI */
#define ss   2
#define mosi 3
#define miso 4
#define sck  5
#define DDRspi DDRB
#endif

#define F_CPU MCLK
