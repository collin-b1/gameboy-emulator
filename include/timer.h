#pragma once

#include <cstdint>

class Timer
{
public:
    Timer();
    uint8_t read(uint16_t);
    void write(uint16_t, uint8_t);
    void tick();
private:
    uint8_t div, tima, tma, tac;
};