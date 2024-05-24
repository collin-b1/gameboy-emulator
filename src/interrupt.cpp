#include "interrupt.h"

InterruptManager::InterruptManager() :
    ie{ 0 },
    iflag{ 0 }
{}

uint8_t InterruptManager::read(uint16_t addr) const
{
    switch (addr)
    {
    case 0xFF0F: return 0 
        | iflag.vblank_req
        | iflag.lcd_req << 1
        | iflag.timer_req << 2
        | iflag.serial_req << 3
        | iflag.joypad_req << 4;

    case 0xFFFF: return 0
        | ie.vblank
        | ie.lcd << 1
        | ie.timer << 2
        | ie.serial << 3
        | ie.joypad << 4;

    default: return 0;
    }
}

void InterruptManager::write(uint16_t addr, uint8_t data)
{
    switch (addr)
    {
    case 0xFF0F:
        iflag.vblank_req = data & 0x01;
        iflag.lcd_req = data & 0x02;
        iflag.timer_req = data & 0x04;
        iflag.serial_req = data & 0x08;
        iflag.joypad_req = data & 0x10;
        break;

    case 0xFFFF: 
        ie.vblank = data & 0x01;
        ie.lcd = data & 0x02;
        ie.timer = data & 0x04;
        ie.serial = data & 0x08;
        ie.joypad = data & 0x10;
        break;

    default: break;
    }
}

uint8_t InterruptManager::interrupt_requested()
{
    return iflag.iflag & ie.ie & 0x3F;
}
