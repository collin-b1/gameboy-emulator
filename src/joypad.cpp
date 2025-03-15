#include "joypad.h"

Joypad::Joypad()
    : joyp(0xFF)
{}

uint8_t Joypad::read(uint16_t addr) const
{
    if (addr == 0xFF00)
    {
        // Upper two bits always return 1
        return 0xC0 | joyp.joyp;
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
        // Only bits 5 and 4 are writable
        joyp.joyp = (data & 0x30) | (joyp.joyp & 0x0F);
    }
}