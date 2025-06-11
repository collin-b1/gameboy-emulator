#pragma once

#include "definitions.h"

class ITickableSystem
{
public:
    virtual ~ITickableSystem() = default;

    virtual void tick(u16) = 0;
    [[nodiscard]] u64 get_cycles() const
    {
        return this->_cycles;
    }

private:
    u64 _cycles{};
};