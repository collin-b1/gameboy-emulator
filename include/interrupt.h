#pragma once

#include <cstdint>

class InterruptManager
{
public:
    InterruptManager();

    uint8_t read(uint16_t addr);
    void write(uint16_t addr, uint8_t data);
private:
    // 0xFF0F: IF - Interrupt Flag
    bool vblank_req, lcd_req, timer_req, serial_req, joypad_req;

    // 0xFFFF: IE - Interrupt Enable
    bool vblank, lcd, timer, serial, joypad;
};