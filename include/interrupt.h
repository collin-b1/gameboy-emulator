#pragma once

#include <cstdint>
#include "memory.h"

enum INTERRUPT_SOURCES
{
    INTERRUPT_VBLANK = 0x40,
    INTERRUPT_STAT = 0x48,
    INTERRUPT_TIMER = 0x50,
    INTERRUPT_SERIAL = 0x58,
    INTERRUPT_JOYPAD = 0x60
};

class InterruptManager : IMemory
{
public:
    InterruptManager();

    uint8_t read(uint16_t addr) const override;
    void write(uint16_t addr, uint8_t data) override;
    uint8_t interrupt_requested();

private:
    // 0xFF0F: IF - Interrupt Flag
    union
    {
        uint8_t iflag;
        struct
        {
            bool _b7, _b6, _b5, vblank_req, lcd_req, timer_req, serial_req, joypad_req;
        };
    } iflag;

    // 0xFFFF: IE - Interrupt Enable
    union
    {
        uint8_t ie;
        struct
        {
            bool _b7, _b6, _b5, vblank, lcd, timer, serial, joypad;
        };
    } ie;
};