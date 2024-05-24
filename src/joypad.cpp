#include "joypad.h"

Joypad::Joypad() :
    joyp()
{}

uint8_t Joypad::read(uint16_t addr)
{
    if (addr == 0xFF00)
    {
        return joyp;
    }
    else
    {
        return 0;
    }
}

void Joypad::write(uint16_t addr, uint8_t data)
{
    if (addr == 0xFF00)
    {
        joyp = (data & 0xF0) | (joyp & 0x0F);
    }
}