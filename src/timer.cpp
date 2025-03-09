#include "timer.h"

#include <iostream>

Timer::Timer(InterruptManager& imu) :
    div(0),
    tima(0),
    tma(0),
    tac(0),
    imu(imu)
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
    case 0xFF04: div = 0; break; // Writing to DIV resets it to 0
    case 0xFF05: tima = data; break;
    case 0xFF06: tma = data; break;
    case 0xFF07: tac = data; break;
    default:
        std::cout << "Invalid timer write at address: " << std::hex << addr << std::endl;
        exit(7);
    }
}

void Timer::tick()
{
    if (tima + tac > 0xFF)
    {
        tima = tma;

        // Request interrupt
        imu.request_interrupt(InterruptSource::INTERRUPT_TIMER);
    }
    else
    {
        tima += tac;
    }
}