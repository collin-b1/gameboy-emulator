#pragma once

#include "definitions.h"
#include "memory.h"

class Joypad : public IMemory
{
public:
    Joypad();

    [[nodiscard]] u8 read(u16) const override;
    void write(u16, u8) override;

private:
    // 0xFF00: P1/JOYP
    union {
        u8 joyp;
        struct
        {
            u8 _unused : 2;
            u8 select_buttons : 1;
            u8 select_dpad : 1;
            u8 start_down : 1;
            u8 select_up : 1;
            u8 b_left : 1;
            u8 a_right : 1;
        };
    } joyp;
};