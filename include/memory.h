#pragma once

#include "definitions.h"

class IMemory
{
public:
    [[nodiscard]] virtual u8 read(u16) const = 0;
    virtual void write(u16, u8) = 0;
    virtual ~IMemory() = default;
};