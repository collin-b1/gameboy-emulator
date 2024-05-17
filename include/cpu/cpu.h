#pragma once
#include <cstdint>
#include "cpu/register.h"
#include "memory/bus.h"

extern uint8_t timings_u[256];
extern uint8_t timings_cb[256];

class CPU
{
public:
	CPU(MemoryBus&);
	//~CPU();

	void reset();
	//void step();

	void next_instruction();
	void decode_opcode(uint8_t);
	void decode_cb_opcode(uint8_t);

	void BIT_b_r(uint8_t bit, uint8_t& r8source);
	void BIT_b_rr(uint8_t bit, uint16_t& r16source);

	void INC_r(uint8_t& r8);

	void JR_cc_e(uint8_t mask, bool flag);

	// Load the value from register r8source into r8dest
	void LD_r_r(uint8_t& r8dest, uint8_t& r8source);

	// Load the value from u8 into r8dest
	void LD_r_n(uint8_t& r8dest);

	// Get value from memory addr (r16source), load into the register at r8dest
	void LD_r_rr(uint8_t& r8dest, uint16_t& r16source);

	// Load the data from r8source into memory addr pointed to by (r16dest) 
	void LD_rr_r(uint16_t &r16dest, uint8_t &r8source);

	// Load u8 into memory addr pointed to by (r16dest) 
	void LD_rr_n(uint16_t &r16dest);

	// Load data from absolute mem addr (u16) into r8dest
	void LD_r_nn(uint8_t &r8dest);

	// Load data from register r8source into mem addr (u16)
	void LD_nn_r(uint8_t &r8source);

	// Load to r8dest the data from mem addr (0xFF00 + r8source)
	void LDH_r_C(uint8_t& r8dest, uint8_t &r8source);
	void LDH_C_r(uint8_t& r8dest, uint8_t& r8source);

	// Load to r8dest the data from mem addr (0xFF00 + n)
	void LDH_r_n(uint8_t &r8dest);

	// Load to (0xFF00 + n) the data from r8source
	void LDH_n_r(uint8_t &r8source);

	// Load to r16dest the 16 bit data u16
	void LD_rr_nn(uint16_t &r16dest);

	// Load to (u16) the data from register r16source
	void LD_nn_rr(uint16_t& r16source);

	// Load to r16dest the data from register r16source
	void LD_rr_rr(uint16_t& r16dest, uint16_t& r16source);

	// Push value r16source onto the stack
	void PUSH_rr(uint16_t& r16source);

	// Pop from stack and load to r16dest
	void POP_rr(uint16_t& r16dest);

	void XOR_r(uint8_t& r8dest);

	void CALL_nn();
	void CALL_cc_nn(uint8_t mask, bool set);
	void RET();
	void RET_cc(uint8_t mask, bool set);
	void RETI();
	void RST_n();
	void HALT();
	void STOP();
	void DI();
	void EI();
	// void NOP();


	Registers get_registers() const;

	void debug_print();
private:
	Registers registers;
	uint8_t cycles;
	uint8_t opcode;
	MemoryBus& mmu;
};