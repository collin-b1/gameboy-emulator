#pragma once

#include <cstdint>

class Joypad
{
public:
    Joypad();

    uint8_t read(uint16_t);
    void write(uint16_t, uint8_t);
private:
    uint8_t joyp;
};