#include "timer.h"

#include <array>
#include <iostream>
#include <format>

constexpr std::array<uint16_t, 4> clock_increment_array{
    256,    // 0x00: 4096 Hz
    4,      // 0x01: 262144 Hz
    16,     // 0x02: 65536 Hz
    64,     // 0x03: 16384 Hz
};

Timer::Timer(InterruptManager& imu)
    : _cycles(0)
    , div(0xABCC)
    , tima(0)
    , tma(0)
    , tac(0)
    , imu(imu)
{}

std::string Timer::get_state() const
{
    return std::format("DIV:{:04X} TIMA:{:02X} TMA:{:02X} TAC:{:02X}",
        div, tima, tma, tac
    );
}

void Timer::tick(uint16_t cycles)
{
    div += static_cast<uint16_t>(cycles);

    if (tac & 0x04)
    {
        uint16_t clock_increment = clock_increment_array[tac & 0x3];

        while (cycles >= clock_increment)
        {
            cycles -= clock_increment;

            if (tima == 0xFF)
            {
                tima = tma;

                // Request interrupt
                imu.request_interrupt(InterruptSource::INTERRUPT_TIMER);
            }
            else
            {
                tima++;
            }
        }
    }
}

uint8_t Timer::read(uint16_t addr) const
{
    switch (addr)
    {
    case 0xFF04: return div >> 8;
    case 0xFF05: return tima;
    case 0xFF06: return tma;
    case 0xFF07: return tac & 0xF7;
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
    case 0xFF07: tac = data & 0xF7; break;
    default:
        std::cout << "Invalid timer write at address: " << std::hex << addr << std::endl;
        exit(7);
    }
}