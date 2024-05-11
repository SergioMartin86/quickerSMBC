#pragma once

#include <cstdint>

extern thread_local bool c;                      /**< Carry flag. */
extern thread_local bool z;                      /**< Zero flag. */
extern thread_local bool n;                      /**< Negative flag. */
extern thread_local bool _storePPUEnabled;

extern inline void setZN(uint8_t value)
{
    z = (value == 0);
    n = (value & (1 << 7)) != 0;
}