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

void CPU::init()
{
    registers.af.word = 0x01B0;
    registers.bc.word = 0x0013;
    registers.de.word = 0x00D8;
    registers.hl.word = 0x014D;
    registers.sp = 0xFFFE;
    registers.pc = 0x0100;
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
    case 0x00: NOP(); break;
    case 0x01: LD_r16_imm16(registers.bc.word); break;
    case 0x02: LD_r16mem_A(registers.bc.word); break;
    case 0x06: LD_r8_imm8(registers.bc.msb); break;
    case 0x0A: LD_A_r16mem(registers.bc.word); break;
    case 0x0C: INC_r8(registers.bc.lsb); break;
    case 0x0E: LD_r8_imm8(registers.bc.lsb); break;
    case 0x11: LD_r16_imm16(registers.de.word); break;
    case 0x12: LD_r16mem_A(registers.de.word); break;
    case 0x16: LD_r8_imm8(registers.de.msb); break;
    case 0x1A: LD_A_r16mem(registers.de.word); break;
    case 0x1E: LD_r8_imm8(registers.de.lsb); break;
    case 0x20: JR_cc_imm8(!registers.get_zero_flag()); break;
    case 0x21: LD_r16_imm16(registers.hl.word); break;
    case 0x22: LD_r16mem_A(registers.hl.word); INC_r16(registers.hl.word); break;
    case 0x26: LD_r8_imm8(registers.hl.msb); break;
    case 0x27: DAA(); break;
    case 0x2A: LD_A_r16mem(registers.hl.word); INC_r16(registers.hl.word); break;
    case 0x2E: LD_r8_imm8(registers.hl.lsb); break;
    case 0x31: LD_r16_imm16(registers.sp); break;
    case 0x32: LD_r16mem_A(registers.hl.word); DEC_r16(registers.hl.word); break;
    case 0x36: LD_r16mem_imm8(registers.hl.word); break;
    case 0x3A: LD_A_r16mem(registers.hl.word); DEC_r16(registers.hl.word); break;
    case 0x3E: LD_r8_imm8(registers.af.msb); break;
    case 0x3C: INC_r8(registers.af.msb); break;
    case 0x76: HALT(); break; // LD (HL), (HL) uniquely halts the CPU
    case 0x7C: LD_r8_r8(registers.af.msb, registers.hl.msb); break;
    case 0xAF: XOR_A_r8(registers.af.msb); break;
    case 0xC0: RET_cc(!registers.get_zero_flag()); break;
    case 0xC1: POP_r16stk(registers.bc.word); break;
    case 0xC3: JP_imm16(); break;
    case 0xC4: CALL_cc_imm16(!registers.get_zero_flag()); break;
    case 0xC5: PUSH_r16stk(registers.bc.word); break;
    case 0xCB:
    {
        std::cout << "Prefixed Opcode found, decoding..." << std::endl;
        uint8_t cb_opcode = mmu.bus_read(registers.pc + 1);
        decode_cb_opcode(cb_opcode);
        registers.pc += timings_cb[cb_opcode];
        break;
    }
    case 0xCC: CALL_cc_imm16(registers.get_zero_flag()); break;
    case 0xCD: CALL_imm16(); break;
    case 0xD1: POP_r16stk(registers.de.word); break;
    case 0xD4: CALL_cc_imm16(!registers.get_carry_flag()); break;
    case 0xD5: PUSH_r16stk(registers.de.word); break;
    case 0xDC: CALL_cc_imm16(registers.get_carry_flag()); break;
    case 0xE1: POP_r16stk(registers.hl.word); break;
    case 0xE2: LDH_C_A(); break;
    case 0xE5: PUSH_r16stk(registers.hl.word); break;
    case 0xF1: POP_r16stk(registers.af.word); break;
    case 0xF2: LDH_A_C(); break;
    case 0xF5: PUSH_r16stk(registers.af.word); break;
    case 0xFB: EI(); break;
    default: 
        std::cout << "Opcode not implemented!" << std::endl;
        exit(1);
    }
}

void CPU::decode_cb_opcode(uint8_t cb_opcode)
{
    std::cout << "\nRetrieved Prefixed Opcode 0x" << std::hex << static_cast<int>(cb_opcode) << std::dec << std::endl;
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
    case 8: BIT_b3_r8(0, *source); break;
    case 9: BIT_b3_r8(1, *source); break;
    case 10: BIT_b3_r8(2, *source); break;
    case 11: BIT_b3_r8(3, *source); break;
    case 12: BIT_b3_r8(4, *source); break;
    case 13: BIT_b3_r8(5, *source); break;
    case 14: BIT_b3_r8(6, *source); break;
    case 15: BIT_b3_r8(7, *source); break;
    default:
        std::cout << "Prefixed Opcode not implemented!" << std::endl;
        exit(1);
    }
    if (hl_direct)
    {
        delete source;
    }
}

void CPU::NOP()
{
    // DO NOTHING!
    // That's right folks--not even one thing.
    // Actually, I have just realized...
    // It does do one thing!
    // It increments the program counter!
    // However, that is done elsewhere...
    // So in other words, nothing.
}

// CALL imm16
void CPU::CALL_imm16()
{
    uint16_t imm16 = mmu.bus_read_word(registers.pc + 1);
    PUSH_r16stk(registers.pc);
    registers.pc = imm16;
}

// CALL cc, imm16
void CPU::CALL_cc_imm16(bool condition_met)
{
    uint16_t imm16 = mmu.bus_read_word(registers.pc + 1);
    if (condition_met)
    {
        PUSH_r16stk(registers.pc);
        registers.pc = imm16;
    }
}

void CPU::RET()
{
    POP_r16stk(registers.pc);
}

void CPU::RET_cc(bool condition_met)
{
    if (condition_met)
    {
        POP_r16stk(registers.pc);
    }
}
void CPU::RETI()
{

}
void CPU::RST_tgt3(uint8_t tgt3)
{
    PUSH_r16stk(registers.pc);
    registers.pc = tgt3;
}

void CPU::HALT()
{
    is_halted = true;
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

// LD [r16], imm8
void CPU::LD_r16mem_imm8(uint16_t& r16)
{
    uint8_t imm8 = mmu.bus_read(registers.pc + 1);
    mmu.bus_write(r16, imm8);
}

// BIT
void CPU::BIT_b3_r8(uint8_t bit, uint8_t &r8source)
{
    uint8_t mask = (1 << bit);
    bool set = !!(mask & r8source);
    registers.set_zero_flag(set);
    registers.set_sub_flag(0);
    registers.set_half_carry_flag(1);
}

// INC r8
void CPU::INC_r8(uint8_t& r8)
{
    uint8_t prev = r8++;
    registers.set_half_carry_flag(Registers::has_half_carry(prev, 1));
    registers.set_sub_flag(1);
    registers.set_zero_flag(r8 == 0);
}

// DEC r8
void CPU::DEC_r8(uint8_t& r8)
{
    bool hc = (((r8 & 0xF) + (++r8 & 0xF)) & 0x10) == 0x10;
    registers.set_half_carry_flag(hc);
    registers.set_sub_flag(0);
    registers.set_zero_flag(r8 == 0);
}

// JR cc, imm8
void CPU::JR_cc_imm8(bool condition_met)
{
    int8_t e = (int8_t) mmu.bus_read(registers.pc + 1);
    if (condition_met)
    {
        registers.pc += e;
    }
}

// JP imm16
void CPU::JP_imm16()
{
    uint16_t nn = mmu.bus_read_word(registers.pc + 1);
    registers.pc = nn;
}

// LD r8, r8
void CPU::LD_r8_r8(uint8_t& r8dest, uint8_t& r8source)
{
    r8dest = r8source;
}

// LD r8, imm8
void CPU::LD_r8_imm8(uint8_t& r8dest)
{
    uint8_t n = mmu.bus_read(registers.pc + 1);
    r8dest = n;
}

// RLCA
void CPU::RLCA()
{
    uint8_t msbit = registers.af.msb >> 7;
    registers.af.word <<= 1;
    registers.af.word |= msbit;

    registers.set_zero_flag(0);
    registers.set_sub_flag(0);
    registers.set_half_carry_flag(0);
    registers.set_carry_flag(msbit);
}

// RRCA
void CPU::RRCA()
{
    uint8_t lsbit = registers.af.msb & 1;
    registers.af.word >>= 1;
    registers.af.word |= (lsbit << 7);

    registers.set_zero_flag(0);
    registers.set_sub_flag(0);
    registers.set_half_carry_flag(0);
    registers.set_carry_flag(lsbit);
}

// RRA
void CPU::RRA()
{
    uint8_t lsbit = registers.af.msb & 1;
    registers.af.word >>= 1;
    registers.af.word |= (uint16_t)registers.get_carry_flag();

    registers.set_zero_flag(0);
    registers.set_sub_flag(0);
    registers.set_half_carry_flag(0);
    registers.set_carry_flag(lsbit);
}

// DAA
void CPU::DAA()
{
    uint8_t correction = 0;
    if ((0xF & registers.af.msb) > 0x9 || registers.get_half_carry_flag())
    {
        correction += 0x06;
    }
    if ((registers.af.msb >> 4) > 0x99 || registers.get_carry_flag())
    {
        correction += 0x60;
        registers.set_carry_flag(1);
    }

    if (registers.get_sub_flag())
    {
        registers.af.msb -= correction;
    }
    else
    {
        registers.af.msb += correction;
    }

    registers.set_half_carry_flag(0);
    registers.set_zero_flag(registers.af.msb == 0);
}

// RLA
void CPU::RLA()
{
    uint8_t msbit = registers.af.msb >> 7;
    registers.af.word <<= 1;
    registers.af.word |= (uint16_t)registers.get_carry_flag();

    registers.set_zero_flag(0);
    registers.set_sub_flag(0);
    registers.set_half_carry_flag(0);
    registers.set_carry_flag(msbit);
}

// LD A, (r16)
void CPU::LD_A_r16mem(uint16_t& r16source)
{
    uint8_t r16mem = mmu.bus_read(r16source);
    registers.af.msb = r16mem;
}

// LDH A, [C]
void CPU::LDH_A_C()
{
    uint8_t ffc = mmu.bus_read(0xFF00 | registers.bc.lsb);
    registers.af.msb = ffc;
}

// LDH A, [imm8]
void CPU::LDH_A_imm8()
{
    uint8_t imm8 = mmu.bus_read(registers.pc + 1);
    uint8_t ffimm8 = mmu.bus_read(0xFF00 | imm8);
    registers.af.msb = ffimm8;
}

// LDH [imm8], A
void CPU::LDH_imm8_A()
{
    uint8_t imm8 = mmu.bus_read(registers.pc + 1);
    mmu.bus_write(0xFF00 | imm8, registers.af.msb);
}

// LDH [C], A
void CPU::LDH_C_A()
{
    mmu.bus_write(0xFF00 | registers.bc.lsb, registers.af.msb);
}

// LD r16, imm16
void CPU::LD_r16_imm16(uint16_t& r16dest)
{
    uint16_t nn = mmu.bus_read_word(registers.pc + 1);
    r16dest = nn;
}

// LD [imm16], SP
void CPU::LD_imm16_SP()
{
    uint16_t imm16 = mmu.bus_read_word(registers.pc + 1);
    mmu.bus_write_word(imm16, registers.sp);
}

// INC r16
void CPU::INC_r16(uint16_t& r16)
{
    ++r16;
}

// DEC r16
void CPU::DEC_r16(uint16_t& r16)
{
    --r16;
}

// ADD HL, r16
void CPU::ADD_HL_r16(uint16_t& r16)
{
    uint16_t sum = registers.hl + r16;
    registers.set_sub_flag(0);
    registers.set_half_carry_flag(Registers::has_half_carry(registers.hl, r16));
    registers.set_carry_flag(Registers::has_carry(registers.hl, r16));
    registers.hl = sum;
}

// LD [imm16], A
void CPU::LD_imm16_A()
{
    uint16_t imm16 = mmu.bus_read_word(registers.pc + 1);
    mmu.bus_write(imm16, registers.af.msb);
}

// LD [r16mem], A
void CPU::LD_r16mem_A(uint16_t& r16dest)
{
    mmu.bus_write(r16dest, registers.af.msb);
}

// LD A, [imm16]
void CPU::LD_A_imm16()
{
    uint16_t imm16 = mmu.bus_read_word(registers.pc + 1);
    uint8_t n = mmu.bus_read(imm16);
    registers.af.msb = n;
}

// ADD SP, imm8
void CPU::ADD_SP_imm8()
{
    // Signed
    uint8_t sp = registers.sp;
    int8_t imm8 = (int8_t) mmu.bus_read(registers.pc + 1);
    registers.set_zero_flag(0);
    registers.set_sub_flag(0);
    registers.set_half_carry_flag(Registers::has_half_carry(sp, imm8));
    registers.set_carry_flag(Registers::has_carry(sp, imm8));
    registers.sp += imm8;
}

// LD HL, SP + imm8
void CPU::LD_HL_SP_imm8()
{
    uint8_t imm8 = mmu.bus_read(registers.pc + 1);
    uint8_t spimm8 = registers.sp + imm8;
    registers.hl = spimm8;
}

// LD SP, HL
void CPU::LD_SP_HL()
{
    registers.sp = registers.hl.word;
}

// PUSH r16stk
void CPU::PUSH_r16stk(uint16_t& r16source)
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

void CPU::POP_r16stk(uint16_t& r16dest)
{
    uint8_t lsb = mmu.bus_read(registers.sp);
    registers.sp++;

    uint8_t msb = mmu.bus_read(registers.sp);
    registers.sp++;

    r16dest = (msb << 8) | lsb;
}

void CPU::XOR_A_r8(uint8_t& r8dest)
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
