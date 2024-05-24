#pragma once

#include <cstdint>
#include "memory.h"

class Joypad : public IMemory
{
public:
    Joypad();

    uint8_t read(uint16_t) const override;
    void write(uint16_t, uint8_t) override;
private:
    // 0xFF00: P1/JOYP
    uint8_t joyp;
};