#include "interrupt.h"

InterruptManager::InterruptManager() :
    ie(0),
    iflag(0)
{}

uint8_t InterruptManager::read(uint16_t addr)
{
    if (addr == 0xFFFF)
    {
        return ie;
    }
    return 0;
}

void InterruptManager::write(uint16_t addr, uint8_t data)
{
    if (addr == 0xFFFF)
    {
        ie = data;
    }
}