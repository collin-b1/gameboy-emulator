#include "serial.h"

#include <iostream>

Serial::Serial() : sb(0), sc(0)
{
}

u8 Serial::read(u16 addr) const
{
    switch (addr)
    {
    case 0xFF01:
        return sb;
    case 0xFF02:
        return sc;
    default:
        std::cout << "Invalid serial read at address: 0x" << std::hex << addr << std::endl;
        exit(7);
    }
}

void Serial::write(u16 addr, u8 data)
{
    switch (addr)
    {
    case 0xFF01:
        sb = data;
        break;
    case 0xFF02:
        sc = data;
        if (sc & 0x80)
        {
            // std::cout << sb;
            sc &= ~0x80;
        }
        break;
    default:
        std::cout << "Invalid serial write at address: 0x" << std::hex << addr << std::endl;
        exit(7);
    }
}