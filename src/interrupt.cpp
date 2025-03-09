#include "interrupt.h"
#include <iostream>

InterruptManager::InterruptManager()
    : ime{ false }
    , ie{ 0 }
    , iflag{ 0 }
{}

uint8_t InterruptManager::read(uint16_t addr) const
{
    switch (addr)
    {
    case 0xFF0F: return 0xE0 | iflag.iflag; // Only lower 5 bits are readible, upper 3 return 1.

    case 0xFFFF: return ie.ie;

    default: return 0;
    }
}

void InterruptManager::write(uint16_t addr, uint8_t data)
{
    switch (addr)
    {
    case 0xFF0F:
        iflag.iflag = 0xE0 | data;
        break;

    case 0xFFFF: 
        ie.ie = data;
        break;

    default: break;
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
    case INTERRUPT_VBLANK: iflag.vblank_req = 1; break;
    case INTERRUPT_LCD_STAT: iflag.lcd_req = 1; break;
    case INTERRUPT_TIMER: iflag.timer_req = 1; break;
    case INTERRUPT_SERIAL: iflag.serial_req = 1; break;
    case INTERRUPT_JOYPAD: iflag.joypad_req = 1; break;
    default: break;
    }
}

void InterruptManager::clear_interrupt_flag(InterruptSource source)
{
    switch (source)
    {
    case INTERRUPT_VBLANK: iflag.vblank_req = 0; break;
    case INTERRUPT_LCD_STAT: iflag.lcd_req = 0; break;
    case INTERRUPT_TIMER: iflag.timer_req = 0; break;
    case INTERRUPT_SERIAL: iflag.serial_req = 0; break;
    case INTERRUPT_JOYPAD: iflag.joypad_req = 0; break;
    default: break;
    }
}

uint8_t InterruptManager::get_interrupt_flag() const
{
    return iflag.iflag;
}

uint8_t InterruptManager::get_interrupt_enable() const
{
    return ie.ie;
}