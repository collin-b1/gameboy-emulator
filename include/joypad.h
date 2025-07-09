#pragma once

#include "definitions.h"
#include "memory.h"

namespace Gobby
{
enum JoypadInput : u8
{
    A = 0,
    B = 1,
    SELECT = 2,
    START = 3,

    RIGHT = 4,
    LEFT = 5,
    UP = 6,
    DOWN = 7,
};
} // namespace Gobby

class Joypad : public IMemory
{
public:
    Joypad();

    [[nodiscard]] u8 read(u16) const override;
    void write(u16, u8) override;

    void set_button_pressed(Gobby::JoypadInput button, bool pressed);

private:
    // 0xFF00: P1/JOYP
    u8 joyp;

    u8 _directions;
    u8 _buttons;
};