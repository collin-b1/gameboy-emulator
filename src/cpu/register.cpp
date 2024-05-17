#include "cpu/register.h"
#include <stdio.h>
#include <iostream>

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

void Registers::print_registers()
{
	printf("%4s %4s %4s %4s %4s %4s\n", "AF", "BC", "DE", "HL", "SP", "PC");
	printf("%02x%02x %02x%02x %02x%02x %02x%02x %04x %04x\n", af.msb, af.lsb, bc.msb, bc.lsb, de.msb, de.lsb, hl.msb, hl.lsb, sp, pc);
}