#include "cpu/register.h"
#include <stdio.h>
#include <iostream>
#include <bitset>

Registers::Registers()
{
	af = 0x0000;
	bc = 0x0000;
	de = 0x0000;
	hl = 0x0000;
	sp = 0x0000;
	pc = 0x0000;
};

bool Registers::get_zero_flag()
{
	return (af.lsb & 0x80) >> 7;
}

bool Registers::get_sub_flag()
{
	return (af.lsb & 0x40) >> 6;
}

bool Registers::get_half_carry_flag()
{
	return (af.lsb & 0x20) >> 5;
}

bool Registers::get_carry_flag()
{
	return (af.lsb & 0x10) >> 4;
}

void Registers::set_zero_flag(bool value)
{
	af.lsb = value ? af.lsb | 0x80 : af.lsb & 0x7F;
}

void Registers::set_sub_flag(bool value)
{
	af.lsb = value ? af.lsb | 0x40 : af.lsb & 0xBF;
}

void Registers::set_half_carry_flag(bool value)
{
	af.lsb = value ? af.lsb | 0x20 : af.lsb & 0xDF;
}

void Registers::set_carry_flag(bool value)
{
	af.lsb = value ? af.lsb | 0x10 : af.lsb & 0xEF;
}

bool Registers::has_half_carry(uint8_t a, uint8_t b)
{
	return (a & 0x0F) + (b & 0x0F) > 0x0F;
}

bool Registers::has_carry(uint8_t a, uint8_t b)
{
	return (a & 0xFF) + (b & 0xFF) > 0xFF;
}

bool Registers::has_half_carry(uint16_t a, uint16_t b)
{
	return (a & 0x0FFF) + (b & 0x0FFF) > 0x0FFF;
}

bool Registers::has_carry(uint16_t a, uint16_t b)
{
	return (a & 0xFFFF) + (b & 0xFFFF) > 0xFFFF;
}

void Registers::print_registers()
{
    std::cout << "CPU State at PC " << std::hex << pc << std::endl;
    std::cout << "A:  0b" << std::bitset<8>(af.msb) << " (0x" << std::hex << static_cast<int>(af.msb) << ")" << std::endl;
    std::cout << "F:  0b" << std::bitset<8>(af.lsb) << " (0x" << std::hex << static_cast<int>(af.lsb) << ")" << std::endl;
    std::cout << "B:  0b" << std::bitset<8>(bc.msb) << " (0x" << std::hex << static_cast<int>(bc.msb) << ")" << std::endl;
    std::cout << "C:  0b" << std::bitset<8>(bc.lsb) << " (0x" << std::hex << static_cast<int>(bc.lsb) << ")" << std::endl;
    std::cout << "D:  0b" << std::bitset<8>(de.msb) << " (0x" << std::hex << static_cast<int>(de.msb) << ")" << std::endl;
    std::cout << "E:  0b" << std::bitset<8>(de.lsb) << " (0x" << std::hex << static_cast<int>(de.lsb) << ")" << std::endl;
    std::cout << "H:  0b" << std::bitset<8>(hl.msb) << " (0x" << std::hex << static_cast<int>(hl.msb) << ")" << std::endl;
    std::cout << "L:  0b" << std::bitset<8>(hl.lsb) << " (0x" << std::hex << static_cast<int>(hl.lsb) << ")" << std::endl;
    std::cout << "SP: 0b" << std::bitset<16>(sp) << " (0x" << std::hex << sp << ")" << std::endl;
    std::cout << "PC: 0b" << std::bitset<16>(pc) << " (0x" << std::hex << pc << ")" << std::endl;

	// printf("%4s %4s %4s %4s %4s %4s\n", "AF", "BC", "DE", "HL", "SP", "PC");
	// printf("%02x%02x %02x%02x %02x%02x %02x%02x %04x %04x\n", af.msb, af.lsb, bc.msb, bc.lsb, de.msb, de.lsb, hl.msb, hl.lsb, sp, pc);
}