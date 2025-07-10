#pragma once

#include "definitions.h"
#include "memory/memory.h"

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

    [[nodiscard]] u8 read(u16 addr) const override;
    void write(u16 addr, u8 data) override;

    void request_interrupt(InterruptSource);
    void clear_interrupt_flag(InterruptSource);

    bool get_ime() const;
    void set_ime(bool);

    [[nodiscard]] u8 get_interrupt_flag() const;
    [[nodiscard]] u8 get_interrupt_enable() const;

private:
    bool ime;

    // 0xFF0F: IF - Interrupt Flag
    union {
        u8 iflag;
        struct
        {
            u8 vblank_req : 1;
            u8 lcd_req : 1;
            u8 timer_req : 1;
            u8 serial_req : 1;
            u8 joypad_req : 1;
            u8 : 3;
        };
    } iflag;

    // 0xFFFF: IE - Interrupt Enable
    union {
        u8 ie;
        struct
        {
            u8 vblank : 1;
            u8 lcd : 1;
            u8 timer : 1;
            u8 serial : 1;
            u8 joypad : 1;
            u8 : 3;
        };
    } ie;
};