#include "joypad.h"
#include <iostream>

Joypad::Joypad(InterruptManager &interrupts)
    : interrupts(interrupts)
    , joyp(0xC0)
    , _buttons(0x0F)    // Button mask
    , _directions(0x0F) // Direction mask
{
}

u8 Joypad::read(u16 addr) const
{

    if (addr == 0xFF00)
    {
        // If neither are selected, lower nibble returns 0xF
        if ((joyp & 0xC0) == 0xC0)
        {
            return joyp | 0x0F;
        }
        else if (joyp == 0)
        {
            return joyp | (_buttons & _directions);
        }
        else if (joyp & 0x10)
        {
            return joyp | _buttons;
        }
        else // (joyp & 0x20)
        {
            return joyp | _directions;
        }
    }
    else
    {
        // Should never happen
        exit(7);
    }
}

void Joypad::write(u16 addr, u8 data)
{
    if (addr == 0xFF00)
    {
        // Only bits 5 and 4 are writable
        joyp = (data & 0x30);
    }
}

void Joypad::set_button_pressed(Gobby::JoypadInput input, bool pressed)
{
    auto inputs_before = read(0xFF00) & 0x0F;
    switch (input)
    {
    case Gobby::A:
    case Gobby::B:
    case Gobby::START:
    case Gobby::SELECT:
        // Unset the button bit
        _buttons &= ~(1 << input);
        // Set the button bit to !pressed
        _buttons |= !pressed << input;
        break;

    case Gobby::LEFT:
    case Gobby::RIGHT:
    case Gobby::UP:
    case Gobby::DOWN:
        // Unset the button bit
        _directions &= ~(1 << (input - 4));
        // Set the button bit to !pressed
        _directions |= !pressed << (input - 4);
        break;
    }
    //    std::cout << "_buttons :: 0x" << std::hex << static_cast<int>(_buttons) << std::endl;
    //    std::cout << "_directions :: 0x" << std::hex << static_cast<int>(_directions) << std::endl;
    //    std::cout << "joyp :: 0x" << std::hex << static_cast<int>(joyp) << std::endl;
    auto inputs_after = read(0xFF00) & 0x0F;
    
    // Check if any bits changed from 1 to 0
    if (inputs_before & ~inputs_after)
    {
        interrupts.request_interrupt(InterruptSource::INTERRUPT_JOYPAD);
    }
}
