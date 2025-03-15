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
    union
    {
        uint8_t joyp;
        struct
        {
            uint8_t _unused : 2;
            uint8_t select_buttons : 1;
            uint8_t select_dpad : 1;
            uint8_t start_down : 1;
            uint8_t select_up : 1;
            uint8_t b_left : 1;
            uint8_t a_right : 1;
        };
    } joyp;
};