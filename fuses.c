#include "config.h"
#include <avr/signature.h>
#include <avr/io.h>

#if FUSE_MEMORY_SIZE == 1
FUSES =
    {
        .low = fuses_low,
    };
#elif FUSE_MEMORY_SIZE == 2
FUSES =
    {
        .low = fuses_low,
        .high = fuses_high,
    };
#elif FUSE_MEMORY_SIZE == 3
FUSES =
    {
        .low = fuses_low,
        .high = fuses_high,
        .extended = fuses_ext,
    };
#else
#error "Unsupported fuse length!"
#endif
