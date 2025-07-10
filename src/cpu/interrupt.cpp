#include "cpu/interrupt.h"
#include <iostream>

InterruptManager::InterruptManager() : ime{0}, ie{0}, iflag{0}
{
}

u8 InterruptManager::read(u16 addr) const
{
    switch (addr)
    {
    case 0xFF0F:
        return 0xE0 | iflag.iflag; // Only lower 5 bits are readable, upper 3 return 1.

    case 0xFFFF:
        return ie.ie;

    default:
        return 0xFF;
    }
}

void InterruptManager::write(u16 addr, u8 data)
{
    switch (addr)
    {
    case 0xFF0F:
        iflag.iflag = 0xE0 | data;
        break;

    case 0xFFFF:
        ie.ie = data;
        break;

    default:
        break;
    }
}

bool InterruptManager::get_ime() const
{
    return ime;
}

void InterruptManager::set_ime(bool enabled)
{
    ime = enabled;
}

void InterruptManager::request_interrupt(InterruptSource source)
{
    switch (source)
    {
    case INTERRUPT_VBLANK:
        iflag.vblank_req = 1;
        break;
    case INTERRUPT_LCD_STAT:
        iflag.lcd_req = 1;
        break;
    case INTERRUPT_TIMER:
        iflag.timer_req = 1;
        break;
    case INTERRUPT_SERIAL:
        iflag.serial_req = 1;
        break;
    case INTERRUPT_JOYPAD:
        iflag.joypad_req = 1;
        break;
    default:
        break;
    }
}

void InterruptManager::clear_interrupt_flag(InterruptSource source)
{
    switch (source)
    {
    case INTERRUPT_VBLANK:
        iflag.vblank_req = 0;
        break;
    case INTERRUPT_LCD_STAT:
        iflag.lcd_req = 0;
        break;
    case INTERRUPT_TIMER:
        iflag.timer_req = 0;
        break;
    case INTERRUPT_SERIAL:
        iflag.serial_req = 0;
        break;
    case INTERRUPT_JOYPAD:
        iflag.joypad_req = 0;
        break;
    default:
        break;
    }
}

u8 InterruptManager::get_interrupt_flag() const
{
    return iflag.iflag;
}

u8 InterruptManager::get_interrupt_enable() const
{
    return ie.ie;
}