#pragma once

#include <cstdint>

enum INTERRUPT_SOURCES
{
    INTERRUPT_VBLANK = 0x40,
    INTERRUPT_STAT = 0x48,
    INTERRUPT_TIMER = 0x50,
    INTERRUPT_SERIAL = 0x58,
    INTERRUPT_JOYPAD = 0x60
};

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