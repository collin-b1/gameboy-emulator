#include "joypad.h"

Joypad::Joypad() : joyp(0xFF)
{
}

u8 Joypad::read(u16 addr) const
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

void Joypad::write(u16 addr, u8 data)
{
    if (addr == 0xFF00)
    {
        // Only bits 5 and 4 are writable
        joyp.joyp = (data & 0x30) | (joyp.joyp & 0x0F);
    }
}