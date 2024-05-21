#pragma once

#include <cstdint>

class InterruptManager
{
public:
    InterruptManager();

    uint8_t read(uint16_t addr);
    void write(uint16_t addr, uint8_t data);
private:
    // 0xFFFF: IE - Interrupt Enable
    uint8_t ie;

    // 0xFF0F: IF - Interrupt Flag
    uint8_t iflag;
};