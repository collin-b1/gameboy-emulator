#include "interrupt.h"

InterruptManager::InterruptManager() :
    vblank(0), lcd(0), timer(0), serial(0), joypad(0),
    vblank_req(0), lcd_req(0), timer_req(0), serial_req(0), joypad_req(0)
{}

uint8_t InterruptManager::read(uint16_t addr)
{
    switch (addr)
    {
    case 0xFF0F: return 0 
        | vblank_req
        | lcd_req << 1
        | timer_req << 2
        | serial_req << 3
        | joypad_req << 4;

    case 0xFFFF: return 0
        | vblank
        | lcd_req << 1
        | timer << 2
        | serial << 3
        | joypad << 4;

    default: return 0;
    }
}

void InterruptManager::write(uint16_t addr, uint8_t data)
{
    switch (addr)
    {
    case 0xFF0F:
        vblank_req = data & 0x01;
        lcd_req = data & 0x02;
        timer_req = data & 0x04;
        serial_req = data & 0x08;
        joypad_req = data & 0x10;
        break;

    case 0xFFFF: 
        vblank = data & 0x01;
        lcd = data & 0x02;
        timer = data & 0x04;
        serial = data & 0x08;
        joypad = data & 0x10;
        break;

    default: break;
    }
}