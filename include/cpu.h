#pragma once
#include <cstdint>
#include "register.h"
#include "interrupt.h"
#include "mmu.h"

extern uint8_t timings_u[256];
extern uint8_t timings_cb[256];

class CPU
{
public:
    CPU(MMU&, InterruptManager&);

    void init_post_boot();

    uint8_t next_instruction();
    uint8_t execute_opcode(uint8_t);
    uint8_t execute_cb_opcode(uint8_t);

    void debug_print();
    void inline_debug_print();

    // Block 0
    void NOP();
    void LD_r16_imm16(uint16_t&);
    void LD_r16mem_A(uint16_t&);
    void LD_A_r16mem(uint16_t&);
    void LD_imm16_SP();
    void INC_r16(uint16_t&);
    void DEC_r16(uint16_t&);
    void ADD_HL_r16(uint16_t&);
    void INC_r8(uint8_t&);
    void DEC_r8(uint8_t&);
    void LD_r8_imm8(uint8_t&);
    void RLCA();
    void RRCA();
    void RLA();
    void RRA();
    void DAA();
    void CPL();
    void SCF();
    void CCF();
    void JR_imm8();
    void JR_cc_imm8(bool);
    void STOP();

    // Block 1
    void LD_r8_r8(uint8_t&, uint8_t&);
    void HALT();

    // Block 2
    void ADD_A_r8(uint8_t&);
    void ADC_A_r8(uint8_t&);
    void SUB_A_r8(uint8_t&);
    void SBC_A_r8(uint8_t&);
    void AND_A_r8(uint8_t&);
    void XOR_A_r8(uint8_t&);
    void OR_A_r8(uint8_t&);
    void CP_A_r8(uint8_t&);
    
    // Block 3
    void ADD_A_imm8();
    void ADC_A_imm8();
    void SUB_A_imm8();
    void SBC_A_imm8();
    void AND_A_imm8();
    void XOR_A_imm8();
    void OR_A_imm8();
    void CP_A_imm8();
    void RET_cc(bool);
    void RET();
    void RETI();
    void JP_cc_imm16(bool);
    void JP_imm16();
    void JP_HL();
    void CALL_cc_imm16(bool);
    void CALL_imm16();
    void RST_tgt3(uint8_t);
    void POP_r16stk(uint16_t&);
    void PUSH_r16stk(uint16_t&);

    // 0xCB prefixed instructions
    void RLC_r8(uint8_t&);
    void RRC_r8(uint8_t&);
    void RL_r8(uint8_t&);
    void RR_r8(uint8_t&);
    void SLA_r8(uint8_t&);
    void SRA_r8(uint8_t&);
    void SWAP_r8(uint8_t&);
    void SRL_r8(uint8_t&);
    void BIT_b3_r8(uint8_t, uint8_t&);
    void RES_b3_r8(uint8_t, uint8_t&);
    void SET_b3_r8(uint8_t, uint8_t&);

    void RLC_r16mem(uint16_t&);
    void RRC_r16mem(uint16_t&);
    void RL_r16mem(uint16_t&);
    void RR_r16mem(uint16_t&);
    void SLA_r16mem(uint16_t&);
    void SRA_r16mem(uint16_t&);
    void SWAP_r16mem(uint16_t&);
    void SRL_r16mem(uint16_t&);
    void BIT_b3_r16mem(uint8_t, uint16_t&);
    void RES_b3_r16mem(uint8_t, uint16_t&);
    void SET_b3_r16mem(uint8_t, uint16_t&);

    void LDH_C_A();
    void LDH_imm8_A();
    void LD_imm16_A();
    void LDH_A_C();
    void LDH_A_imm8();
    void LD_A_imm16();
    void ADD_SP_imm8();
    void LD_HL_SP_imm8();
    void LD_SP_HL();
    void DI();
    void EI();

    // Additional instructions
    void LD_r16mem_imm8(uint16_t&);
    void INC_r16mem(uint16_t&);
    void DEC_r16mem(uint16_t&);
    void LD_r8_r16mem(uint8_t&, uint16_t&);
    void LD_r16mem_r8(uint16_t&, uint8_t&);
    void ADD_A_r16mem(uint16_t&);
    void ADC_A_r16mem(uint16_t&);
    void SUB_A_r16mem(uint16_t&);
    void SBC_A_r16mem(uint16_t&);
    void AND_A_r16mem(uint16_t&);
    void XOR_A_r16mem(uint16_t&);
    void OR_A_r16mem(uint16_t&);
    void CP_A_r16mem(uint16_t&);
private:
    Registers registers;
    bool ime_req, ime, is_halted, is_stopped;
    uint8_t opcode;
    MMU& mmu;
    InterruptManager& imu;
};