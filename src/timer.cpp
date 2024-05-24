#include "timer.h"

#include <iostream>

Timer::Timer() :
    div(0),
    tima(0),
    tma(0),
    tac(0)
{}

uint8_t Timer::read(uint16_t addr) const
{
    switch (addr)
    {
    case 0xFF04: return div;
    case 0xFF05: return tima;
    case 0xFF06: return tma;
    case 0xFF07: return tac;
    default: 
        std::cout << "Invalid timer read at address: " << std::hex << addr << std::endl;
        exit(7);
    }
    return 0;
}

void Timer::write(uint16_t addr, uint8_t data)
{
    switch (addr)
    {
    case 0xFF04:
    {
        div = 0;
        break;
    }
    case 0xFF05: tima = data; break;
    case 0xFF06: tma = data; break;
    case 0xFF07: tac = data; break;
    default:
        std::cout << "Invalid timer read at address: " << std::hex << addr << std::endl;
        exit(7);
    }
}

void Timer::tick()
{
    if (tima + tac > 0xFF)
    {
        tima = tma;
        // TODO: Request interrupt
    }
    else
    {
        tima += tac;
    }
}