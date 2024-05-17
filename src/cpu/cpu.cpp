#include "cpu/cpu.h"
#include <iostream>
#define Z 0x10000000u // Zero flag
#define N 0x01000000u // Subtraction flag
#define H 0x00100000u // Half Carry flag
#define C 0x00010000u // Carry flag

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
	case 0x02: LD_rr_r(registers.bc.word, registers.af.msb); break;
	case 0x06: LD_r_n(registers.bc.msb); break;
	case 0x0A: LD_r_rr(registers.af.msb, registers.bc.word); break;
	case 0x0C: INC_r(registers.bc.lsb); break;
	case 0x0E: LD_r_n(registers.bc.lsb); break;
	case 0x11: LD_rr_nn(registers.de.word); break;
	case 0x12: LD_rr_r(registers.de.word, registers.af.msb); break;
	case 0x16: LD_r_n(registers.de.msb); break;
	case 0x1A: LD_r_rr(registers.af.msb, registers.de.word); break;
	case 0x1E: LD_r_n(registers.de.lsb); break;
	case 0x20: JR_cc_e(Z, 0); break;
	case 0x21: LD_rr_nn(registers.hl.word); break;
	case 0x22: LD_rr_r(registers.hl.word, registers.af.msb); registers.hl.word++; break;
	case 0x26: LD_r_n(registers.hl.msb); break;
	case 0x2A: LD_r_rr(registers.af.msb, registers.hl.word); registers.hl.word++; break;
	case 0x2E: LD_r_n(registers.hl.lsb); break;
	case 0x31: LD_rr_nn(registers.sp); break;
	case 0x32: LD_rr_r(registers.hl.word, registers.af.msb); registers.hl.word--; break;
	case 0x36: LD_rr_n(registers.hl.word); break;
	case 0x3A: LD_r_rr(registers.af.msb, registers.hl.word); registers.hl.word--; break;
	case 0x3E: LD_r_n(registers.af.msb); break;
	case 0x7C: LD_r_r(registers.af.msb, registers.hl.msb); break;
	case 0xAF: XOR_r(registers.af.msb); break;
	case 0xC0: RET_cc(Z, 0); break;
	case 0xC1: POP_rr(registers.bc.word); break;
	case 0xC4: CALL_cc_nn(Z, false); break;
	case 0xC5: PUSH_rr(registers.bc.word); break;
	case 0xCB:
	{
		uint8_t cb_opcode = mmu.bus_read(registers.pc + 1);
		decode_cb_opcode(cb_opcode);
		registers.pc += timings_cb[cb_opcode];
		break;
	}
	case 0xCC: CALL_cc_nn(Z, true); break;
	case 0xCD: CALL_nn(); break;
	case 0xD1: POP_rr(registers.de.word); break;
	case 0xD4: CALL_cc_nn(C, false); break;
	case 0xD5: PUSH_rr(registers.de.word); break;
	case 0xDC: CALL_cc_nn(C, true); break;
	case 0xE1: POP_rr(registers.hl.word); break;
	case 0xE2: LDH_C_r(registers.bc.lsb, registers.af.msb); break;
	case 0xE5: PUSH_rr(registers.hl.word); break;
	case 0xF1: POP_rr(registers.af.word); break;
	case 0xF2: LDH_r_C(registers.af.msb, registers.bc.lsb); break;
	case 0xF5: PUSH_rr(registers.af.word); break;
	case 0xFB: EI(); break;

	default: // 
		std::cerr << "Invalid opcode 0x" << std::hex << static_cast<int>(opcode) << " at PC " << registers.pc << std::dec << std::endl;
		exit(1);
	}
}

void CPU::decode_cb_opcode(uint8_t cb_opcode)
{
	std::cout << "\nRetrieved Prefixed Opcode 0x" << std::hex << static_cast<int>(cb_opcode) << std::dec << std::endl;
	switch (cb_opcode)
	{
	default:
		// https://izik1.github.io/gbops/

		uint8_t* source = nullptr;
		bool hl_direct = false;

		int col = cb_opcode % 0x08;
		int row = cb_opcode / 0x08;

		std::cout << row << ", " << col << std::endl;

		switch (col)
		{
		case 0: source = &(registers.bc.msb); break;
		case 1: source = &(registers.bc.lsb); break;
		case 2: source = &(registers.de.msb); break;
		case 3: source = &(registers.de.lsb); break;
		case 4: source = &(registers.hl.msb); break;
		case 5: source = &(registers.hl.lsb); break;
		case 6: source = new uint8_t(mmu.bus_read(registers.hl)); hl_direct = true; break;
		case 7: source = &(registers.af.msb); break;
		default:
			std::cout << "Prefixed Opcode not implemented!" << std::endl;
			exit(1);
		}

		switch (row)
		{
		case 8: BIT_b_r(0, *source); break;
		case 9: BIT_b_r(1, *source); break;
		case 10: BIT_b_r(2, *source); break;
		case 11: BIT_b_r(3, *source); break;
		case 12: BIT_b_r(4, *source); break;
		case 13: BIT_b_r(5, *source); break;
		case 14: BIT_b_r(6, *source); break;
		case 15: BIT_b_r(7, *source); break;
		default:
		{
			std::cout << "Prefixed Opcode not implemented!" << std::endl;
			exit(1);
		}
		}
		if (hl_direct)
		{
			delete source;
		}
	}
}

// Control instructions
void CPU::CALL_nn()
{
	uint16_t nn = mmu.bus_read_word(registers.pc + 1);
	PUSH_rr(registers.pc);
	registers.pc = nn;
}

void CPU::CALL_cc_nn(uint8_t mask, bool set)
{
	uint16_t nn = mmu.bus_read_word(registers.pc + 1);
	if (!!(registers.af.lsb & mask) == set)
	{
		PUSH_rr(registers.pc);
		registers.pc = nn;
	}
}

void CPU::RET()
{
	POP_rr(registers.pc);
}

void CPU::RET_cc(uint8_t mask, bool set)
{
	if (!!(registers.af.lsb & mask) == set)
	{
		POP_rr(registers.pc);
	}
}
void CPU::RETI()
{

}
void CPU::RST_n()
{

}
void CPU::HALT()
{

}
void CPU::STOP()
{

}
void CPU::DI()
{

}
void CPU::EI()
{

}

void CPU::BIT_b_r(uint8_t bit, uint8_t &r8source)
{
	uint8_t mask = (1 << bit);
	bool set = !!(mask & r8source);
	registers.set_zero_flag(set);
	registers.set_sub_flag(0);
	registers.set_half_carry_flag(1);
}

void CPU::BIT_b_rr(uint8_t bit, uint16_t& r16source)
{
	uint8_t byte = mmu.bus_read(r16source);
	uint8_t mask = (1 << bit);
	bool set = !!(mask & byte);
	registers.set_zero_flag(set);
	registers.set_sub_flag(0);
	registers.set_half_carry_flag(1);
}

void CPU::INC_r(uint8_t& r8)
{
	bool hc = (((r8 & 0xF) + (++r8 & 0xF)) & 0x10) == 0x10;
	registers.set_half_carry_flag(hc);
	registers.set_sub_flag(0);
	registers.set_zero_flag(r8 == 0);
}

void CPU::JR_cc_e(uint8_t mask, bool flag)
{
	int8_t e = (int8_t) mmu.bus_read(registers.pc + 1);
	if (!!(registers.af.lsb & mask) == flag)
	{
		registers.pc += e;
	}
}

void CPU::LD_r_r(uint8_t& r8dest, uint8_t& r8source)
{
	r8dest = r8source;
}

void CPU::LD_r_n(uint8_t& r8dest)
{
	uint8_t n = mmu.bus_read(registers.pc + 1);
	r8dest = n;
}

void CPU::LD_r_rr(uint8_t& r8dest, uint16_t& r16source)
{
	uint8_t rr = mmu.bus_read(r16source);
	r8dest = rr;
}

void CPU::LDH_r_C(uint8_t& r8dest, uint8_t& r8source)
{
	r8dest = mmu.bus_read(0xFF00 | r8source);
}

void CPU::LDH_C_r(uint8_t& r8dest, uint8_t& r8source)
{
	mmu.bus_write(0xFF00 | r8source, r8source);
}

void CPU::LDH_r_n(uint8_t& r8dest)
{
	uint8_t n = mmu.bus_read(registers.pc + 1);
	uint8_t ffn = mmu.bus_read(0xFF00 | n);
	r8dest = ffn;
}

void CPU::LDH_n_r(uint8_t& r8source)
{
	uint8_t n = mmu.bus_read(registers.pc + 1);
	mmu.bus_write(0xFF00 | n, r8source);
}

void CPU::LD_rr_nn(uint16_t& r16dest)
{
	uint16_t nn = mmu.bus_read_word(registers.pc + 1);
	r16dest = nn;
}

void CPU::LD_nn_rr(uint16_t& r16source)
{
	uint16_t nn = mmu.bus_read_word(registers.pc + 1);
	mmu.bus_write_word(nn, r16source);
}

void CPU::LD_rr_rr(uint16_t& r16dest, uint16_t& r16source)
{
	r16dest = r16source;
}

void CPU::LD_rr_r(uint16_t& r16dest, uint8_t& r8source)
{
	mmu.bus_write(r16dest, r8source);
}

void CPU::LD_rr_n(uint16_t& r16dest)
{
	uint8_t n = mmu.bus_read(registers.pc + 1);
	mmu.bus_write(r16dest, n);
}

void CPU::LD_r_nn(uint8_t& r8dest)
{
	uint16_t nn = mmu.bus_read_word(registers.pc + 1);
	uint8_t n = mmu.bus_read(nn);
	r8dest = n;
}

void CPU::LD_nn_r(uint8_t& r8source)
{
	uint16_t nn = mmu.bus_read_word(registers.pc + 1);
	mmu.bus_write(nn, r8source);
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
