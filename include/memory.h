#pragma once

#include <cstdint>

class IMemory
{
public:
    virtual uint8_t read(uint16_t) const = 0;
    virtual void write(uint16_t, uint8_t) = 0;
    virtual ~IMemory() = default;
};