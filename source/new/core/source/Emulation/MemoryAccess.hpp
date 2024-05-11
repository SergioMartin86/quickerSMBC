#ifndef MEMORYACCESS_HPP
#define MEMORYACCESS_HPP

#include <cstdint>
#include <common.hpp>

class SMBEngine;

/**
 * Wraps operations to memory values/registers so that status flags can be set for branch operations.
 */
class MemoryAccess
{
public:
MemoryAccess(SMBEngine& engine, uint8_t* value) :
    engine(engine)
{
    this->value = value;
}

MemoryAccess(SMBEngine& engine, uint8_t constant) :
    engine(engine)
{
    this->constant = constant;
    this->value = &this->constant;
}

inline MemoryAccess& operator = (uint8_t value)
{
    *(this->value) = value;
    setZN(value);
    return *this;
}

inline MemoryAccess& operator = (const MemoryAccess& rhs)
{
    return ((*this) = *(rhs.value));
}

inline MemoryAccess& operator += (uint8_t value)
{
    uint16_t temp = *(this->value) + value + (c ? 1 : 0);
    *(this->value) = temp & 0xff;
    setZN(*(this->value));
    c = temp > 0xff;
    return *this;
}

inline MemoryAccess& operator -= (uint8_t value)
{
    uint16_t temp = *(this->value) - value - (c ? 0 : 1);
    *(this->value) = (temp & 0xff);
    setZN(*(this->value));
    c = temp < 0x100;
    return *this;
}

inline MemoryAccess& operator ++ ()
{
    *(this->value) = *(this->value) + 1;
    setZN(*(this->value));
    return *this;
}

inline MemoryAccess& operator -- ()
{
    *(this->value) = *(this->value) - 1;
    setZN(*(this->value));
    return *this;
}

inline MemoryAccess& operator ++ (int unused)
{
    return ++(*this);
}

inline MemoryAccess& operator -- (int unused)
{
    return --(*this);
}

inline MemoryAccess& operator &= (uint8_t value)
{
    *(this->value) &= value;
    setZN(*(this->value));
    return *this;
}

inline MemoryAccess& operator |= (uint8_t value)
{
    *(this->value) |= value;
    setZN(*(this->value));
    return *this;
}

inline MemoryAccess& operator ^= (uint8_t value)
{
    *(this->value) ^= value;
    setZN(*(this->value));
    return *this;
}

inline MemoryAccess& operator <<= (int shift)
{
    for (int i = 0; i < shift; i++)
    {
        c = *(this->value) & (1 << 7);
        *(this->value) = (*(this->value) << 1) & 0xfe;
        setZN(*(this->value));
    }
    return *this;
}

inline MemoryAccess& operator >>= (int shift)
{
    for (int i = 0; i < shift; i++)
    {
        c = *(this->value) & (1 << 0);
        *(this->value) = (*(this->value) >> 1) & 0x7f;
        setZN(*(this->value));
    }
    return *this;
}

inline operator uint8_t()
{
    return *value;
}

inline void rol()
{
    bool bit7 = *(this->value) & (1 << 7);
    *(this->value) <<= 1;
    if( c )
    {
        *(this->value) |= (1 << 0);
    }
    c = bit7;
    setZN(*(this->value));
}

inline void ror()
{
    bool bit0 = *(this->value) & (1 << 0);
    *(this->value) >>= 1;
    if( c )
    {
        *(this->value) |= (1 << 7);
    }
    c = bit0;
    setZN(*(this->value));
}

    SMBEngine& engine;
    uint8_t* value;
    uint8_t constant;
};

#endif // MEMORYACCESS_HPP
