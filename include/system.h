#pragma once

#include "definitions.h"

class ITickableSystem
{
public:
    virtual ~ITickableSystem() = default;

    virtual void tick(u16) = 0;
};