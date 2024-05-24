#pragma once

#include <cstdint>
#include "memory.h"

class Timer : public IMemory
{
public:
    Timer();
    uint8_t read(uint16_t) const override;
    void write(uint16_t, uint8_t) override;
    void tick();
private:
    uint8_t div, tima, tma, tac;
};