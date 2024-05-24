#include "serial.h"

#include <iostream>

Serial::Serial() :
    sb(0), sc(0)
{}

uint8_t Serial::read(uint16_t addr) const
{
    switch (addr)
    {
    case 0xFF01: return sb;
    case 0xFF02: return sc;
    default: 
        std::cout << "Invalid serial read at address: 0x" << std::hex << addr << std::endl;
        exit(7);
    }
}

void Serial::write(uint16_t addr, uint8_t data)
{
    switch (addr)
    {
    case 0xFF01: sb = data; break;
    case 0xFF02: sc = data; break;
    default:
        std::cout << "Invalid serial write at address: 0x" << std::hex << addr << std::endl;
        exit(7);
    }
}