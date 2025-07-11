#include "timer.h"

#include <array>
#include <format>
#include <iostream>

constexpr std::array<u16, 4> clock_increment_array{
    256, // 0x00: 4096 Hz
    4,   // 0x01: 262144 Hz
    16,  // 0x02: 65536 Hz
    64,  // 0x03: 16384 Hz
};

Timer::Timer(InterruptManager &imu)
    : _div_counter(0), _tima_counter(0), _div_stopped(false), div(0), tima(0), tma(0), tac(0), imu(imu)
{
}

std::string Timer::get_state() const
{
    return std::format("DIV:{:04X} TIMA:{:02X} TMA:{:02X} TAC:{:02X}", div, tima, tma, tac);
}

void Timer::stop_div(bool stopped)
{
    _div_stopped = stopped;
}

void Timer::tick(u16 cycles)
{
    if (_div_stopped) // CPU is stopped
        return;

    _div_counter += cycles;

    if (_div_counter >= 256)
    {
        _div_counter -= 256;
        ++div;
    }

    if (tac & 0x04) // TAC bit 4: Timer enable
    {
        const u16 clock_increment = clock_increment_array[tac & 0x3];
        _tima_counter += cycles;

        while (_tima_counter >= clock_increment)
        {
            _tima_counter -= clock_increment;

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

u8 Timer::read(u16 addr) const
{
    switch (addr)
    {
    case 0xFF04:
        return div >> 8;
    case 0xFF05:
        return tima;
    case 0xFF06:
        return tma;
    case 0xFF07:
        return tac;
    default:
        std::cout << "Invalid timer read at address: " << std::hex << addr << std::endl;
        exit(7);
    }
    return 0;
}

void Timer::write(u16 addr, u8 data)
{
    switch (addr)
    {
    case 0xFF04:
        div = 0; // Writing to DIV resets it to 0
        break;
    case 0xFF05:
        tima = data;
        break;
    case 0xFF06:
        tma = data;
        break;
    case 0xFF07:
        tac = data & 0x07;
        break;
    default:
        std::cout << "Invalid timer write at address: " << std::hex << addr << std::endl;
        exit(7);
    }
}