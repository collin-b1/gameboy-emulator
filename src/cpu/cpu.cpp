#include "cpu/cpu.h"
#include <iostream>
#define Z 0x10000000 // Zero flag
#define N 0x01000000 // Subtraction flag
#define H 0x00100000 // Half Carry flag
#define C 0x00010000 // Carry flag

// Unprefixed
uint8_t timings_u[256] = {
//  00 01 02 03 04 05 06 07 08 09 0A 0B 0C 0D 0E 0F
	1, 3, 2, 2, 1, 1, 2, 1, 5, 2, 2, 2, 1, 1, 2, 1, // 0x00
	1, 3, 2, 2, 1, 1, 2, 1, 3, 2, 2, 2, 1, 1, 2, 1, // 0x10
	3, 3, 2, 2, 1, 1, 2, 1, 3, 2, 2, 2, 1, 1, 2, 1, // 0x20
	3, 3, 2, 2, 3, 3, 3, 1, 3, 2, 2, 2, 1, 1, 2, 1, // 0x30
	1, 1, 1, 1, 1, 1, 2, 1, 1, 1, 1, 1, 1, 1, 2, 1, // 0x40
	1, 1, 1, 1, 1, 1, 1, 2, 1, 1, 1, 1, 1, 1, 2, 1, // 0x50
	1, 1, 1, 1, 1, 1, 2, 1, 1, 1, 1, 1, 1, 1, 2, 1, // 0x60
	2, 2, 2, 2, 2, 2, 1, 2, 1, 1, 1, 1, 1, 1, 2, 1, // 0x70
	1, 1, 1, 1, 1, 1, 2, 1, 1, 1, 1, 1, 1, 1, 2, 1, // 0x80
	1, 1, 1, 1, 1, 1, 2, 1, 1, 1, 1, 1, 1, 1, 2, 1, // 0x90
	1, 1, 1, 1, 1, 1, 2, 1, 1, 1, 1, 1, 1, 1, 2, 1, // 0xA0
	1, 1, 1, 1, 1, 1, 2, 1, 1, 1, 1, 1, 1, 1, 2, 1, // 0xB0
	2, 3, 3, 4, 3, 4, 2, 4, 2, 4, 3, 1, 3, 6, 2, 4, // 0xC0
	2, 3, 3, 0, 3, 4, 2, 4, 2, 4, 3, 0, 3, 0, 2, 4, // 0xD0
	3, 3, 2, 0, 0, 4, 2, 4, 4, 1, 4, 0, 0, 0, 2, 4, // 0xE0
	3, 3, 2, 1, 0, 4, 2, 4, 3, 2, 4, 1, 0, 0, 2, 4, // 0xF0
};

// 0xCB Prefixed
uint8_t timings_cb[256] = {
//  00 01 02 03 04 05 06 07 08 09 0A 0B 0C 0D 0E 0F
	2, 2, 2, 2, 2, 2, 4, 2, 2, 2, 2, 2, 2, 2, 4, 2, // 0x00
	2, 2, 2, 2, 2, 2, 4, 2, 2, 2, 2, 2, 2, 2, 4, 2, // 0x10
	2, 2, 2, 2, 2, 2, 4, 2, 2, 2, 2, 2, 2, 2, 4, 2, // 0x20
	2, 2, 2, 2, 2, 2, 4, 2, 2, 2, 2, 2, 2, 2, 4, 2, // 0x30
	2, 2, 2, 2, 2, 2, 4, 2, 2, 2, 2, 2, 2, 2, 4, 2, // 0x40
	2, 2, 2, 2, 2, 2, 3, 2, 2, 2, 2, 2, 2, 2, 3, 2, // 0x50
	2, 2, 2, 2, 2, 2, 3, 2, 2, 2, 2, 2, 2, 2, 3, 2, // 0x60
	2, 2, 2, 2, 2, 2, 3, 2, 2, 2, 2, 2, 2, 2, 3, 2, // 0x70
	2, 2, 2, 2, 2, 2, 4, 2, 2, 2, 2, 2, 2, 2, 4, 2, // 0x80
	2, 2, 2, 2, 2, 2, 4, 2, 2, 2, 2, 2, 2, 2, 4, 2, // 0x90
	2, 2, 2, 2, 2, 2, 4, 2, 2, 2, 2, 2, 2, 2, 4, 2, // 0xA0
	2, 2, 2, 2, 2, 2, 4, 2, 2, 2, 2, 2, 2, 2, 4, 2, // 0xB0
	2, 2, 2, 2, 2, 2, 4, 2, 2, 2, 2, 2, 2, 2, 4, 2, // 0xC0
	2, 2, 2, 2, 2, 2, 4, 2, 2, 2, 2, 2, 2, 2, 4, 2, // 0xD0
	2, 2, 2, 2, 2, 2, 4, 2, 2, 2, 2, 2, 2, 2, 4, 2, // 0xE0
	2, 2, 2, 2, 2, 2, 4, 2, 2, 2, 2, 2, 2, 2, 4, 2, // 0xF0
};

CPU::CPU(MemoryBus &mmu) : registers(), cycles(0), opcode(0), mmu(mmu)
{}

void CPU::reset()
{
	// need to implement
}

void CPU::next_instruction()
{
	uint8_t opcode = mmu.bus_read(registers.pc);

	
	debug_print(); // debug only

	decode_opcode(opcode);

	registers.pc += timings_u[opcode];
}

void CPU::decode_opcode(uint8_t opcode)
{
	std::cout << "\nApplying Opcode 0x" << std::hex << static_cast<int>(opcode) << std::dec << std::endl;
	switch (opcode)
	{
	case 0x00: break; // NOP
	case 0x01: LD_rr_nn(registers.bc.word); break;
	case 0x11: LD_rr_nn(registers.de.word); break;
	case 0x20: JR_cc_e(Z, 0); break;
	case 0x21: LD_rr_nn(registers.hl.word); break;
	case 0x31: LD_rr_nn(registers.sp); break;
	case 0x32: LD_rr_r(registers.hl.word, registers.af.msb); registers.hl.word--;  break;
	case 0x7C: LD_r_r(registers.af.msb, registers.hl.msb); break;
	case 0xAF: XOR_r(registers.af.msb); break;
	case 0xCB:
	{
		uint8_t cb_opcode = mmu.bus_read(registers.pc + 1);
		decode_cb_opcode(cb_opcode);
		registers.pc += timings_cb[cb_opcode];
		break;
	}

	default: // 
		std::cerr << "Invalid opcode 0x" << std::hex << static_cast<int>(opcode) << " at PC " << registers.pc << std::dec << std::endl;
		exit(1);
	}
}

void CPU::decode_cb_opcode(uint8_t cb_opcode)
{
	std::cout << "\Retrieved Prefixed Opcode 0x" << std::hex << static_cast<int>(cb_opcode) << std::dec << std::endl;
	switch (cb_opcode)
	{
	default:
		std::cout << "Prefixed Opcode not implemented!" << std::endl;
		exit(1);
	}
}

void CPU::JR_cc_e(uint8_t mask, bool flag)
{
	int8_t e = static_cast<int8_t>(mmu.bus_read(registers.pc + 1));
	if (!!(registers.af.lsb & mask) == flag)
	{
		registers.pc += e;
	}
}

void CPU::LD_r_r(uint8_t& r8dest, uint8_t& r8source)
{
	r8dest = r8source;
}

void CPU::LDH_r_r(uint8_t& r8dest, uint8_t& r8source)
{
	r8dest = mmu.bus_read(0xFF00 + r8source);
}

void CPU::LD_rr_nn(uint16_t& r16dest)
{
	uint16_t nn = mmu.bus_read_word(registers.pc + 1);
	r16dest = nn;
}

void CPU::LD_rr_r(uint16_t& r16dest, uint8_t& r8source)
{
	mmu.bus_write(r16dest, r8source);
}

void CPU::PUSH_rr(uint16_t& r16source)
{
	// Get most significant byte and add to stack
	registers.sp--;
	uint8_t msb = (r16source >> 8) & 0xFF;
	mmu.bus_write(registers.sp, msb);

	// get least significant byte and add to stack
	registers.sp--;
	uint8_t lsb = r16source & 0xFF;
	mmu.bus_write(registers.sp, lsb);
}

void CPU::POP_rr(uint16_t& r16dest)
{
	uint8_t lsb = mmu.bus_read(registers.sp);
	registers.sp++;

	uint8_t msb = mmu.bus_read(registers.sp);
	registers.sp++;

	r16dest = (msb << 8) | lsb;
}

void CPU::XOR_r(uint8_t& r8dest)
{
	registers.af.msb ^= r8dest;
	registers.set_zero_flag(registers.af.msb == 0);
	registers.set_sub_flag(0);
	registers.set_carry_flag(0);
	registers.set_half_carry_flag(0);
}

Registers CPU::get_registers() const
{
	return Registers();
}

void CPU::debug_print()
{
	registers.print_registers();
}
