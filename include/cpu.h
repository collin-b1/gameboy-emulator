#pragma once
#include "definitions.h"
#include "interrupt.h"
#include "mmu.h"
#include "register.h"
#include <cstdint>
#include <string>

struct Instruction
{
    std::string mneumonic;
};

class CPU
{
public:
    CPU(MMU &, InterruptManager &);

    void init_post_boot();

    u8 next_instruction();
    u8 execute_opcode(u8);
    u8 execute_cb_opcode(u8);

    void handle_interrupts();
    void service_interrupt(InterruptSource);

    void debug_print() const;
    [[nodiscard]] std::string get_state() const;

    // Block 0
    void NOP();
    void LD_r16_imm16(u16 &);
    void LD_r16mem_A(u16 &);
    void LD_A_r16mem(u16 &);
    void LD_imm16_SP();
    void INC_r16(u16 &);
    void DEC_r16(u16 &);
    void ADD_HL_r16(u16 &);
    void INC_r8(u8 &);
    void DEC_r8(u8 &);
    void LD_r8_imm8(u8 &);
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
    void LD_r8_r8(u8 &, u8 &);
    void HALT();

    // Block 2
    void ADD_A_r8(u8 &);
    void ADC_A_r8(u8 &);
    void SUB_A_r8(u8 &);
    void SBC_A_r8(u8 &);
    void AND_A_r8(u8 &);
    void XOR_A_r8(u8 &);
    void OR_A_r8(u8 &);
    void CP_A_r8(u8 &);

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
    void RST_tgt3(u8);
    void POP_r16stk(u16 &);
    void PUSH_r16stk(u16 &);

    // 0xCB prefixed instructions
    void RLC_r8(u8 &);
    void RRC_r8(u8 &);
    void RL_r8(u8 &);
    void RR_r8(u8 &);
    void SLA_r8(u8 &);
    void SRA_r8(u8 &);
    void SWAP_r8(u8 &);
    void SRL_r8(u8 &);
    void BIT_b3_r8(u8, u8 &);
    void RES_b3_r8(u8, u8 &);
    void SET_b3_r8(u8, u8 &);

    void RLC_r16mem(u16 &);
    void RRC_r16mem(u16 &);
    void RL_r16mem(u16 &);
    void RR_r16mem(u16 &);
    void SLA_r16mem(u16 &);
    void SRA_r16mem(u16 &);
    void SWAP_r16mem(u16 &);
    void SRL_r16mem(u16 &);
    void BIT_b3_r16mem(u8, u16 &);
    void RES_b3_r16mem(u8, u16 &);
    void SET_b3_r16mem(u8, u16 &);

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
    void LD_r16mem_imm8(u16 &);
    void INC_r16mem(u16 &);
    void DEC_r16mem(u16 &);
    void LD_r8_r16mem(u8 &, u16 &);
    void LD_r16mem_r8(u16 &, u8 &);
    void ADD_A_r16mem(u16 &);
    void ADC_A_r16mem(u16 &);
    void SUB_A_r16mem(u16 &);
    void SBC_A_r16mem(u16 &);
    void AND_A_r16mem(u16 &);
    void XOR_A_r16mem(u16 &);
    void OR_A_r16mem(u16 &);
    void CP_A_r16mem(u16 &);

private:
    Registers registers;
    bool is_halted, is_stopped;
    u8 ime_scheduler;
    u8 opcode;
    MMU &mmu;
    InterruptManager &imu;
};