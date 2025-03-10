#pragma once

#include <cstdint>
#include "memory.h"

enum InterruptSource
{
    INTERRUPT_VBLANK = 0x40,
    INTERRUPT_LCD_STAT = 0x48,
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

    void request_interrupt(InterruptSource);
    void clear_interrupt_flag(InterruptSource);

    bool get_ime() const;
    void set_ime(bool);

    uint8_t get_interrupt_flag() const;
    uint8_t get_interrupt_enable() const;
    

private:
    bool ime;

    // 0xFF0F: IF - Interrupt Flag
    union
    {
        uint8_t iflag;
        struct
        {
            uint8_t vblank_req : 1;
            uint8_t lcd_req : 1;
            uint8_t timer_req : 1;
            uint8_t serial_req : 1;
            uint8_t joypad_req : 1;
            uint8_t _unused : 3;
        };
    } iflag;

    // 0xFFFF: IE - Interrupt Enable
    union
    {
        uint8_t ie;
        struct
        {
            uint8_t vblank : 1;
            uint8_t lcd : 1;
            uint8_t timer : 1;
            uint8_t serial : 1;
            uint8_t joypad : 1;
            uint8_t _unused : 3;
        };
    } ie;
};