#include "cpu.h"
#include <iostream>
#include <array>
#include <format>

// Unprefixed
constexpr std::array<uint8_t, 256> timings_u{
//  00 01 02 03 04 05 06 07 08 09 0A 0B 0C 0D 0E 0F
    1, 3, 2, 2, 1, 1, 2, 1, 5, 2, 2, 2, 1, 1, 2, 1, // 0x00
    1, 3, 2, 2, 1, 1, 2, 1, 3, 2, 2, 2, 1, 1, 2, 1, // 0x10
    2, 3, 2, 2, 1, 1, 2, 1, 2, 2, 2, 2, 1, 1, 2, 1, // 0x20
    2, 3, 2, 2, 3, 3, 3, 1, 2, 2, 2, 2, 1, 1, 2, 1, // 0x30
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
constexpr std::array<uint8_t, 256> timings_cb{
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

CPU::CPU(MMU& mmu, InterruptManager& imu)
    : registers()
    , opcode(0)
    , mmu(mmu)
    , imu(imu)
    , ime_scheduler(false)
    , is_halted(false) 
    , is_stopped(false)
{}

// Set registers to post-bootrom values
void CPU::init_post_boot()
{
    registers.af.word = 0x01B0;
    registers.bc.word = 0x0013;
    registers.de.word = 0x00D8;
    registers.hl.word = 0x014D;
    registers.sp = 0xFFFE;
    registers.pc = 0x0100;
}

uint8_t CPU::next_instruction()
{
    auto iflag = imu.get_interrupt_flag();
    auto ie = imu.get_interrupt_enable();
    auto ime = imu.get_ime();

    // ie & iflag != 0 should unhalt regardless of ime
    if (is_halted)
    {
        if (ime || (ie & iflag))
        {
            is_halted = false;
        }
        else
        {
            return 1;
        }
    }

    if (ime && (iflag & ie))
    {
        handle_interrupts();

        // 6 M-cycles used for handling interrupts
        return 6;
    }

    // Handle 1-cycle delay from EI instruction
    // IME doesn't get set until END of cycle AFTER EI
    if (ime_scheduler)
    {
        imu.set_ime(true);
        ime_scheduler = false;
    }
    
    // Fetch, decode, and execute opcode
    auto opcode = mmu.bus_read(registers.pc++);
    auto cycles = execute_opcode(opcode);

    //debug_print();

    return cycles;
}

void CPU::handle_interrupts()
{
    auto iflag = imu.get_interrupt_flag();
    auto ie = imu.get_interrupt_enable();
    auto ime = imu.get_ime();

    if (!ime) return;
    
    if (iflag & ie & 0x1)
    {
        service_interrupt(InterruptSource::INTERRUPT_VBLANK);
    }
    else if (iflag & ie & 0x2)
    {
        service_interrupt(InterruptSource::INTERRUPT_LCD_STAT);
    }
    else if (iflag & ie & 0x4)
    {
        service_interrupt(InterruptSource::INTERRUPT_TIMER);
    }
    else if (iflag & ie & 0x8)
    {
        service_interrupt(InterruptSource::INTERRUPT_SERIAL);
    }
    else if (iflag & ie & 0x10)
    {
        service_interrupt(InterruptSource::INTERRUPT_JOYPAD);
    }
}

void CPU::service_interrupt(InterruptSource source)
{
    // Push PC to stack
    PUSH_r16stk(registers.pc);

    // Clear interrupt source flag
    imu.clear_interrupt_flag(source);

    // Set IME to 0
    imu.set_ime(false);

    // Set PC to interrupt vector
    registers.pc = 0x00FF & source;
}

void CPU::debug_print()
{
    std::cout << get_state() << std::endl;
}

std::string CPU::get_state() const
{
    return std::format("AF:{:04X} BC:{:04X} DE:{:04X} HL:{:04X} SP:{:04X} PC:{:04X} PCMEM:{:02X},{:02X},{:02X},{:02X}",
        registers.af.word,
        registers.bc.word,
        registers.de.word,
        registers.hl.word,
        registers.sp, 
        registers.pc, 
        mmu.bus_read(registers.pc), 
        mmu.bus_read(registers.pc + 1), 
        mmu.bus_read(registers.pc + 2), 
        mmu.bus_read(registers.pc + 3)
    );
}

uint8_t CPU::execute_opcode(uint8_t opcode)
{
    uint8_t cycles = timings_u[opcode];
    // TODO: Generalize rows to blocks of 8 to make less redundant.
    switch (opcode)
    {
    case 0x00: NOP(); break;
    case 0x01: LD_r16_imm16(registers.bc.word); break;
    case 0x02: LD_r16mem_A(registers.bc.word); break;
    case 0x03: INC_r16(registers.bc.word); break;
    case 0x04: INC_r8(registers.bc.msb); break;
    case 0x05: DEC_r8(registers.bc.msb); break;
    case 0x06: LD_r8_imm8(registers.bc.msb); break;
    case 0x07: RLCA(); break;
    case 0x08: LD_imm16_SP(); break;
    case 0x09: ADD_HL_r16(registers.bc.word); break;
    case 0x0A: LD_A_r16mem(registers.bc.word); break;
    case 0x0B: DEC_r16(registers.bc.word); break;
    case 0x0C: INC_r8(registers.bc.lsb); break;
    case 0x0D: DEC_r8(registers.bc.lsb); break;
    case 0x0E: LD_r8_imm8(registers.bc.lsb); break;
    case 0x0F: RRCA(); break;
    case 0x10: STOP(); break;
    case 0x11: LD_r16_imm16(registers.de.word); break;
    case 0x12: LD_r16mem_A(registers.de.word); break;
    case 0x13: INC_r16(registers.de.word); break;
    case 0x14: INC_r8(registers.de.msb); break;
    case 0x15: DEC_r8(registers.de.msb); break;
    case 0x16: LD_r8_imm8(registers.de.msb); break;
    case 0x17: RLA(); break;
    case 0x18: JR_imm8(); break;
    case 0x19: ADD_HL_r16(registers.de.word); break;
    case 0x1A: LD_A_r16mem(registers.de.word); break;
    case 0x1B: DEC_r16(registers.de.word); break;
    case 0x1C: INC_r8(registers.de.lsb); break;
    case 0x1D: DEC_r8(registers.de.lsb); break;
    case 0x1E: LD_r8_imm8(registers.de.lsb); break;
    case 0x1F: RRA(); break;
    case 0x20: JR_cc_imm8(!registers.get_zero_flag()); break;
    case 0x21: LD_r16_imm16(registers.hl.word); break;
    case 0x22: LD_r16mem_A(registers.hl.word); INC_r16(registers.hl.word); break;
    case 0x23: INC_r16(registers.hl.word); break;
    case 0x24: INC_r8(registers.hl.msb); break;
    case 0x25: DEC_r8(registers.hl.msb); break;
    case 0x26: LD_r8_imm8(registers.hl.msb); break;
    case 0x27: DAA(); break;
    case 0x28: JR_cc_imm8(registers.get_zero_flag()); break;
    case 0x29: ADD_HL_r16(registers.hl.word); break;
    case 0x2A: LD_A_r16mem(registers.hl.word); INC_r16(registers.hl.word); break;
    case 0x2B: DEC_r16(registers.hl.word); break;
    case 0x2C: INC_r8(registers.hl.lsb); break;
    case 0x2D: DEC_r8(registers.hl.lsb); break;
    case 0x2E: LD_r8_imm8(registers.hl.lsb); break;
    case 0x2F: CPL(); break;
    case 0x30: JR_cc_imm8(!registers.get_carry_flag()); break;
    case 0x31: LD_r16_imm16(registers.sp); break;
    case 0x32: LD_r16mem_A(registers.hl.word); DEC_r16(registers.hl.word); break;
    case 0x33: INC_r16(registers.sp); break;
    case 0x34: INC_r16mem(registers.hl.word); break;
    case 0x35: DEC_r16mem(registers.hl.word); break;
    case 0x36: LD_r16mem_imm8(registers.hl.word); break;
    case 0x37: SCF(); break;
    case 0x38: JR_cc_imm8(registers.get_carry_flag()); break;
    case 0x39: ADD_HL_r16(registers.sp); break;
    case 0x3A: LD_A_r16mem(registers.hl.word); DEC_r16(registers.hl.word); break;
    case 0x3B: DEC_r16(registers.sp); break;
    case 0x3C: INC_r8(registers.af.msb); break;
    case 0x3D: DEC_r8(registers.af.msb); break;
    case 0x3E: LD_r8_imm8(registers.af.msb); break;
    case 0x3F: CCF(); break;

    case 0x40: LD_r8_r8(registers.bc.msb, registers.bc.msb); break;
    case 0x41: LD_r8_r8(registers.bc.msb, registers.bc.lsb); break;
    case 0x42: LD_r8_r8(registers.bc.msb, registers.de.msb); break;
    case 0x43: LD_r8_r8(registers.bc.msb, registers.de.lsb); break;
    case 0x44: LD_r8_r8(registers.bc.msb, registers.hl.msb); break;
    case 0x45: LD_r8_r8(registers.bc.msb, registers.hl.lsb); break;
    case 0x46: LD_r8_r16mem(registers.bc.msb, registers.hl.word); break;
    case 0x47: LD_r8_r8(registers.bc.msb, registers.af.msb); break;

    case 0x48: LD_r8_r8(registers.bc.lsb, registers.bc.msb); break;
    case 0x49: LD_r8_r8(registers.bc.lsb, registers.bc.lsb); break;
    case 0x4A: LD_r8_r8(registers.bc.lsb, registers.de.msb); break;
    case 0x4B: LD_r8_r8(registers.bc.lsb, registers.de.lsb); break;
    case 0x4C: LD_r8_r8(registers.bc.lsb, registers.hl.msb); break;
    case 0x4D: LD_r8_r8(registers.bc.lsb, registers.hl.lsb); break;
    case 0x4E: LD_r8_r16mem(registers.bc.lsb, registers.hl.word); break;
    case 0x4F: LD_r8_r8(registers.bc.lsb, registers.af.msb); break;

    case 0x50: LD_r8_r8(registers.de.msb, registers.bc.msb); break;
    case 0x51: LD_r8_r8(registers.de.msb, registers.bc.lsb); break;
    case 0x52: LD_r8_r8(registers.de.msb, registers.de.msb); break;
    case 0x53: LD_r8_r8(registers.de.msb, registers.de.lsb); break;
    case 0x54: LD_r8_r8(registers.de.msb, registers.hl.msb); break;
    case 0x55: LD_r8_r8(registers.de.msb, registers.hl.lsb); break;
    case 0x56: LD_r8_r16mem(registers.de.msb, registers.hl.word); break;
    case 0x57: LD_r8_r8(registers.de.msb, registers.af.msb); break;

    case 0x58: LD_r8_r8(registers.de.lsb, registers.bc.msb); break;
    case 0x59: LD_r8_r8(registers.de.lsb, registers.bc.lsb); break;
    case 0x5A: LD_r8_r8(registers.de.lsb, registers.de.msb); break;
    case 0x5B: LD_r8_r8(registers.de.lsb, registers.de.lsb); break;
    case 0x5C: LD_r8_r8(registers.de.lsb, registers.hl.msb); break;
    case 0x5D: LD_r8_r8(registers.de.lsb, registers.hl.lsb); break;
    case 0x5E: LD_r8_r16mem(registers.de.lsb, registers.hl.word); break;
    case 0x5F: LD_r8_r8(registers.de.lsb, registers.af.msb); break;

    case 0x60: LD_r8_r8(registers.hl.msb, registers.bc.msb); break;
    case 0x61: LD_r8_r8(registers.hl.msb, registers.bc.lsb); break;
    case 0x62: LD_r8_r8(registers.hl.msb, registers.de.msb); break;
    case 0x63: LD_r8_r8(registers.hl.msb, registers.de.lsb); break;
    case 0x64: LD_r8_r8(registers.hl.msb, registers.hl.msb); break;
    case 0x65: LD_r8_r8(registers.hl.msb, registers.hl.lsb); break;
    case 0x66: LD_r8_r16mem(registers.hl.msb, registers.hl.word); break;
    case 0x67: LD_r8_r8(registers.hl.msb, registers.af.msb); break;

    case 0x68: LD_r8_r8(registers.hl.lsb, registers.bc.msb); break;
    case 0x69: LD_r8_r8(registers.hl.lsb, registers.bc.lsb); break;
    case 0x6A: LD_r8_r8(registers.hl.lsb, registers.de.msb); break;
    case 0x6B: LD_r8_r8(registers.hl.lsb, registers.de.lsb); break;
    case 0x6C: LD_r8_r8(registers.hl.lsb, registers.hl.msb); break;
    case 0x6D: LD_r8_r8(registers.hl.lsb, registers.hl.lsb); break;
    case 0x6E: LD_r8_r16mem(registers.hl.lsb, registers.hl.word); break;
    case 0x6F: LD_r8_r8(registers.hl.lsb, registers.af.msb); break;

    case 0x70: LD_r16mem_r8(registers.hl.word, registers.bc.msb); break;
    case 0x71: LD_r16mem_r8(registers.hl.word, registers.bc.lsb); break;
    case 0x72: LD_r16mem_r8(registers.hl.word, registers.de.msb); break;
    case 0x73: LD_r16mem_r8(registers.hl.word, registers.de.lsb); break;
    case 0x74: LD_r16mem_r8(registers.hl.word, registers.hl.msb); break;
    case 0x75: LD_r16mem_r8(registers.hl.word, registers.hl.lsb); break;
    case 0x76: HALT(); break; // LD (HL), (HL) uniquely halts the CPU
    case 0x77: LD_r16mem_r8(registers.hl.word, registers.af.msb); break;

    case 0x78: LD_r8_r8(registers.af.msb, registers.bc.msb); break;
    case 0x79: LD_r8_r8(registers.af.msb, registers.bc.lsb); break;
    case 0x7A: LD_r8_r8(registers.af.msb, registers.de.msb); break;
    case 0x7B: LD_r8_r8(registers.af.msb, registers.de.lsb); break;
    case 0x7C: LD_r8_r8(registers.af.msb, registers.hl.msb); break;
    case 0x7D: LD_r8_r8(registers.af.msb, registers.hl.lsb); break;
    case 0x7E: LD_r8_r16mem(registers.af.msb, registers.hl.word); break;
    case 0x7F: LD_r8_r8(registers.af.msb, registers.af.msb); break;

    case 0x80: ADD_A_r8(registers.bc.msb); break;
    case 0x81: ADD_A_r8(registers.bc.lsb); break;
    case 0x82: ADD_A_r8(registers.de.msb); break;
    case 0x83: ADD_A_r8(registers.de.lsb); break;
    case 0x84: ADD_A_r8(registers.hl.msb); break;
    case 0x85: ADD_A_r8(registers.hl.lsb); break;
    case 0x86: ADD_A_r16mem(registers.hl.word); break;
    case 0x87: ADD_A_r8(registers.af.msb); break;

    case 0x88: ADC_A_r8(registers.bc.msb); break;
    case 0x89: ADC_A_r8(registers.bc.lsb); break;
    case 0x8A: ADC_A_r8(registers.de.msb); break;
    case 0x8B: ADC_A_r8(registers.de.lsb); break;
    case 0x8C: ADC_A_r8(registers.hl.msb); break;
    case 0x8D: ADC_A_r8(registers.hl.lsb); break;
    case 0x8E: ADC_A_r16mem(registers.hl.word); break;
    case 0x8F: ADC_A_r8(registers.af.msb); break;

    case 0x90: SUB_A_r8(registers.bc.msb); break;
    case 0x91: SUB_A_r8(registers.bc.lsb); break;
    case 0x92: SUB_A_r8(registers.de.msb); break;
    case 0x93: SUB_A_r8(registers.de.lsb); break;
    case 0x94: SUB_A_r8(registers.hl.msb); break;
    case 0x95: SUB_A_r8(registers.hl.lsb); break;
    case 0x96: SUB_A_r16mem(registers.hl.word); break;
    case 0x97: SUB_A_r8(registers.af.msb); break;

    case 0x98: SBC_A_r8(registers.bc.msb); break;
    case 0x99: SBC_A_r8(registers.bc.lsb); break;
    case 0x9A: SBC_A_r8(registers.de.msb); break;
    case 0x9B: SBC_A_r8(registers.de.lsb); break;
    case 0x9C: SBC_A_r8(registers.hl.msb); break;
    case 0x9D: SBC_A_r8(registers.hl.lsb); break;
    case 0x9E: SBC_A_r16mem(registers.hl.word); break;
    case 0x9F: SBC_A_r8(registers.af.msb); break;

    case 0xA0: AND_A_r8(registers.bc.msb); break;
    case 0xA1: AND_A_r8(registers.bc.lsb); break;
    case 0xA2: AND_A_r8(registers.de.msb); break;
    case 0xA3: AND_A_r8(registers.de.lsb); break;
    case 0xA4: AND_A_r8(registers.hl.msb); break;
    case 0xA5: AND_A_r8(registers.hl.lsb); break;
    case 0xA6: AND_A_r16mem(registers.hl.word); break;
    case 0xA7: AND_A_r8(registers.af.msb); break;

    case 0xA8: XOR_A_r8(registers.bc.msb); break;
    case 0xA9: XOR_A_r8(registers.bc.lsb); break;
    case 0xAA: XOR_A_r8(registers.de.msb); break;
    case 0xAB: XOR_A_r8(registers.de.lsb); break;
    case 0xAC: XOR_A_r8(registers.hl.msb); break;
    case 0xAD: XOR_A_r8(registers.hl.lsb); break;
    case 0xAE: XOR_A_r16mem(registers.hl.word); break;
    case 0xAF: XOR_A_r8(registers.af.msb); break;

    case 0xB0: OR_A_r8(registers.bc.msb); break;
    case 0xB1: OR_A_r8(registers.bc.lsb); break;
    case 0xB2: OR_A_r8(registers.de.msb); break;
    case 0xB3: OR_A_r8(registers.de.lsb); break;
    case 0xB4: OR_A_r8(registers.hl.msb); break;
    case 0xB5: OR_A_r8(registers.hl.lsb); break;
    case 0xB6: OR_A_r16mem(registers.hl.word); break;
    case 0xB7: OR_A_r8(registers.af.msb); break;

    case 0xB8: CP_A_r8(registers.bc.msb); break;
    case 0xB9: CP_A_r8(registers.bc.lsb); break;
    case 0xBA: CP_A_r8(registers.de.msb); break;
    case 0xBB: CP_A_r8(registers.de.lsb); break;
    case 0xBC: CP_A_r8(registers.hl.msb); break;
    case 0xBD: CP_A_r8(registers.hl.lsb); break;
    case 0xBE: CP_A_r16mem(registers.hl.word); break;
    case 0xBF: CP_A_r8(registers.af.msb); break;

    case 0xC0: RET_cc(!registers.get_zero_flag()); break;
    case 0xC1: POP_r16stk(registers.bc.word); break;
    case 0xC2: JP_cc_imm16(!registers.get_zero_flag()); break;
    case 0xC3: JP_imm16(); break;
    case 0xC4: CALL_cc_imm16(!registers.get_zero_flag()); break;
    case 0xC5: PUSH_r16stk(registers.bc.word); break;
    case 0xC6: ADD_A_imm8(); break;
    case 0xC7: RST_tgt3(0x00); break;

    case 0xC8: RET_cc(registers.get_zero_flag()); break;
    case 0xC9: RET(); break;
    case 0xCA: JP_cc_imm16(registers.get_zero_flag()); break;
    case 0xCB:
    {
        uint8_t cb_opcode = mmu.bus_read(registers.pc++);
        cycles += execute_cb_opcode(cb_opcode);
        break;
    }
    case 0xCC: CALL_cc_imm16(registers.get_zero_flag()); break;
    case 0xCD: CALL_imm16(); break;
    case 0xCE: ADC_A_imm8(); break;
    case 0xCF: RST_tgt3(0x08); break;
    case 0xD0: RET_cc(!registers.get_carry_flag()); break;
    case 0xD1: POP_r16stk(registers.de.word); break;
    case 0xD2: JP_cc_imm16(!registers.get_carry_flag()); break;
    case 0xD4: CALL_cc_imm16(!registers.get_carry_flag()); break;
    case 0xD5: PUSH_r16stk(registers.de.word); break;
    case 0xD6: SUB_A_imm8(); break;
    case 0xD7: RST_tgt3(0x10); break;
    case 0xD8: RET_cc(registers.get_carry_flag()); break;
    case 0xD9: RETI(); break;
    case 0xDA: JP_cc_imm16(registers.get_carry_flag()); break;
    case 0xDC: CALL_cc_imm16(registers.get_carry_flag()); break;
    case 0xDE: SBC_A_imm8(); break;
    case 0xDF: RST_tgt3(0x18); break;
    case 0xE0: LDH_imm8_A(); break;
    case 0xE1: POP_r16stk(registers.hl.word); break;
    case 0xE2: LDH_C_A(); break;
    case 0xE5: PUSH_r16stk(registers.hl.word); break;
    case 0xE6: AND_A_imm8(); break;
    case 0xE7: RST_tgt3(0x20); break;
    case 0xE8: ADD_SP_imm8(); break;
    case 0xE9: JP_HL(); break;
    case 0xEA: LD_imm16_A(); break;
    case 0xEE: XOR_A_imm8(); break;
    case 0xEF: RST_tgt3(0x28); break;
    case 0xF0: LDH_A_imm8(); break;
    case 0xF1: POP_r16stk(registers.af.word); registers.af.lsb &= 0xF0; break;
    case 0xF2: LDH_A_C(); break;
    case 0xF3: DI(); break;
    case 0xF5: PUSH_r16stk(registers.af.word); break;
    case 0xF6: OR_A_imm8(); break;
    case 0xF7: RST_tgt3(0x30); break;
    case 0xF8: LD_HL_SP_imm8(); break;
    case 0xF9: LD_SP_HL(); break;
    case 0xFA: LD_A_imm16(); break;
    case 0xFB: EI(); break;
    case 0xFE: CP_A_imm8(); break;
    case 0xFF: RST_tgt3(0x38); break;
    default:
        std::cout << "Illegal opcode: " << std::hex << (int)opcode << std::endl;
        exit(1);
    }

    return cycles;
}

uint8_t CPU::execute_cb_opcode(uint8_t cb_opcode)
{
    switch (cb_opcode)
    {
    case 0x00: RLC_r8(registers.bc.msb); break;
    case 0x01: RLC_r8(registers.bc.lsb); break;
    case 0x02: RLC_r8(registers.de.msb); break;
    case 0x03: RLC_r8(registers.de.lsb); break;
    case 0x04: RLC_r8(registers.hl.msb); break;
    case 0x05: RLC_r8(registers.hl.lsb); break;
    case 0x06: RLC_r16mem(registers.hl.word); break;
    case 0x07: RLC_r8(registers.af.msb); break;

    case 0x08: RRC_r8(registers.bc.msb); break;
    case 0x09: RRC_r8(registers.bc.lsb); break;
    case 0x0A: RRC_r8(registers.de.msb); break;
    case 0x0B: RRC_r8(registers.de.lsb); break;
    case 0x0C: RRC_r8(registers.hl.msb); break;
    case 0x0D: RRC_r8(registers.hl.lsb); break;
    case 0x0E: RRC_r16mem(registers.hl.word); break;
    case 0x0F: RRC_r8(registers.af.msb); break;

    case 0x10: RL_r8(registers.bc.msb); break;
    case 0x11: RL_r8(registers.bc.lsb); break;
    case 0x12: RL_r8(registers.de.msb); break;
    case 0x13: RL_r8(registers.de.lsb); break;
    case 0x14: RL_r8(registers.hl.msb); break;
    case 0x15: RL_r8(registers.hl.lsb); break;
    case 0x16: RL_r16mem(registers.hl.word); break;
    case 0x17: RL_r8(registers.af.msb); break;

    case 0x18: RR_r8(registers.bc.msb); break;
    case 0x19: RR_r8(registers.bc.lsb); break;
    case 0x1A: RR_r8(registers.de.msb); break;
    case 0x1B: RR_r8(registers.de.lsb); break;
    case 0x1C: RR_r8(registers.hl.msb); break;
    case 0x1D: RR_r8(registers.hl.lsb); break;
    case 0x1E: RR_r16mem(registers.hl.word); break;
    case 0x1F: RR_r8(registers.af.msb); break;

    case 0x20: SLA_r8(registers.bc.msb); break;
    case 0x21: SLA_r8(registers.bc.lsb); break;
    case 0x22: SLA_r8(registers.de.msb); break;
    case 0x23: SLA_r8(registers.de.lsb); break;
    case 0x24: SLA_r8(registers.hl.msb); break;
    case 0x25: SLA_r8(registers.hl.lsb); break;
    case 0x26: SLA_r16mem(registers.hl.word); break;
    case 0x27: SLA_r8(registers.af.msb); break;

    case 0x28: SRA_r8(registers.bc.msb); break;
    case 0x29: SRA_r8(registers.bc.lsb); break;
    case 0x2A: SRA_r8(registers.de.msb); break;
    case 0x2B: SRA_r8(registers.de.lsb); break;
    case 0x2C: SRA_r8(registers.hl.msb); break;
    case 0x2D: SRA_r8(registers.hl.lsb); break;
    case 0x2E: SRA_r16mem(registers.hl.word); break;
    case 0x2F: SRA_r8(registers.af.msb); break;

    case 0x30: SWAP_r8(registers.bc.msb); break;
    case 0x31: SWAP_r8(registers.bc.lsb); break;
    case 0x32: SWAP_r8(registers.de.msb); break;
    case 0x33: SWAP_r8(registers.de.lsb); break;
    case 0x34: SWAP_r8(registers.hl.msb); break;
    case 0x35: SWAP_r8(registers.hl.lsb); break;
    case 0x36: SWAP_r16mem(registers.hl.word); break;
    case 0x37: SWAP_r8(registers.af.msb); break;
    
    case 0x38: SRL_r8(registers.bc.msb); break;
    case 0x39: SRL_r8(registers.bc.lsb); break;
    case 0x3A: SRL_r8(registers.de.msb); break;
    case 0x3B: SRL_r8(registers.de.lsb); break;
    case 0x3C: SRL_r8(registers.hl.msb); break;
    case 0x3D: SRL_r8(registers.hl.lsb); break;
    case 0x3E: SRL_r16mem(registers.hl.word); break;
    case 0x3F: SRL_r8(registers.af.msb); break;

    default:
    {
        uint8_t bit = (cb_opcode & 0b00111000) >> 3;
        if (cb_opcode >= 0x40 && cb_opcode <= 0x7F)
        {
            switch (cb_opcode % 0x08)
            {
            case 0x00: BIT_b3_r8(bit, registers.bc.msb); break;
            case 0x01: BIT_b3_r8(bit, registers.bc.lsb); break;
            case 0x02: BIT_b3_r8(bit, registers.de.msb); break;
            case 0x03: BIT_b3_r8(bit, registers.de.lsb); break;
            case 0x04: BIT_b3_r8(bit, registers.hl.msb); break;
            case 0x05: BIT_b3_r8(bit, registers.hl.lsb); break;
            case 0x06: BIT_b3_r16mem(bit, registers.hl.word); break;
            case 0x07: BIT_b3_r8(bit, registers.af.msb); break;
            }
        }
        else if (cb_opcode >= 0x80 && cb_opcode <= 0xBF)
        {
            switch (cb_opcode % 0x08)
            {
            case 0x00: RES_b3_r8(bit, registers.bc.msb); break;
            case 0x01: RES_b3_r8(bit, registers.bc.lsb); break;
            case 0x02: RES_b3_r8(bit, registers.de.msb); break;
            case 0x03: RES_b3_r8(bit, registers.de.lsb); break;
            case 0x04: RES_b3_r8(bit, registers.hl.msb); break;
            case 0x05: RES_b3_r8(bit, registers.hl.lsb); break;
            case 0x06: RES_b3_r16mem(bit, registers.hl.word); break;
            case 0x07: RES_b3_r8(bit, registers.af.msb); break;
            }
        }
        else if (cb_opcode >= 0xC0 && cb_opcode <= 0xFF)
        {
            switch (cb_opcode % 0x08)
            {
            case 0x00: SET_b3_r8(bit, registers.bc.msb); break;
            case 0x01: SET_b3_r8(bit, registers.bc.lsb); break;
            case 0x02: SET_b3_r8(bit, registers.de.msb); break;
            case 0x03: SET_b3_r8(bit, registers.de.lsb); break;
            case 0x04: SET_b3_r8(bit, registers.hl.msb); break;
            case 0x05: SET_b3_r8(bit, registers.hl.lsb); break;
            case 0x06: SET_b3_r16mem(bit, registers.hl.word); break;
            case 0x07: SET_b3_r8(bit, registers.af.msb); break;
            }
        }
        else
        {
            std::cout << "Illegal CB opcode: " << std::hex << (int)cb_opcode << std::endl;
            exit(1);
        }
    }
    }
    return timings_cb[cb_opcode];
}

void CPU::NOP()
{}

// CALL imm16
void CPU::CALL_imm16()
{
    uint16_t imm16 = mmu.bus_read_word(registers.pc);
    registers.pc += 2;

    PUSH_r16stk(registers.pc);
    registers.pc = imm16;
}

// CALL cc, imm16
void CPU::CALL_cc_imm16(bool condition_met)
{
    uint16_t imm16 = mmu.bus_read_word(registers.pc);
    registers.pc += 2;

    if (condition_met)
    {
        PUSH_r16stk(registers.pc);
        registers.pc = imm16;
    }
}

// RET
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
    RET();

    // RETI doesn't have the 1 cycle delay that EI has
    imu.set_ime(true);
}

void CPU::JP_cc_imm16(bool condition_met)
{
    uint16_t nn = mmu.bus_read_word(registers.pc);
    registers.pc += 2;

    if (condition_met)
    {
        registers.pc = nn;
    }
}

void CPU::RST_tgt3(uint8_t tgt3)
{
    PUSH_r16stk(registers.pc);
    registers.pc = tgt3;
}

void CPU::HALT()
{
    auto ie = imu.get_interrupt_enable();
    auto iflag = imu.get_interrupt_flag();
    if (ie & iflag) // ie & if & 0x1F != 0
    {
        if (!imu.get_ime())
        {
            //halt_bug = 1;
        }
    }
    else // ie & if & 0x1F == 0
    {
        is_halted = 1;
    }
}

void CPU::ADD_A_r8(uint8_t& r8)
{
    uint8_t result = registers.af.msb + r8;
    
    registers.set_zero_flag(result == 0);
    registers.set_sub_flag(0);
    registers.set_half_carry_flag(Registers::addition_half_carry(registers.af.msb, r8));
    registers.set_carry_flag(Registers::addition_carry(registers.af.msb, r8));

    registers.af.msb = result;
}

void CPU::ADC_A_r8(uint8_t& r8)
{
    uint8_t carry = registers.get_carry_flag();
    uint8_t result = registers.af.msb + r8 + carry;
    
    registers.set_zero_flag(result == 0);
    registers.set_sub_flag(0);

    bool has_half_carry =
        Registers::addition_half_carry(registers.af.msb, r8) ||
        Registers::addition_half_carry(registers.af.msb, carry) ||
        Registers::addition_half_carry(registers.af.msb + carry, r8);

    bool has_carry =
        Registers::addition_carry(registers.af.msb, r8) ||
        Registers::addition_carry(registers.af.msb, carry) ||
        Registers::addition_carry(registers.af.msb + carry, r8);

    registers.set_half_carry_flag(has_half_carry);
    registers.set_carry_flag(has_carry);

    registers.af.msb = result;
}

void CPU::SUB_A_r8(uint8_t& r8)
{
    uint8_t result = registers.af.msb - r8;
    
    registers.set_zero_flag(result == 0);
    registers.set_sub_flag(1);
    registers.set_half_carry_flag(Registers::subtraction_half_carry(registers.af.msb, r8));
    registers.set_carry_flag(Registers::subtraction_carry(registers.af.msb, r8));

    registers.af.msb = result;
}

void CPU::SBC_A_r8(uint8_t& r8)
{
    uint8_t carry = registers.get_carry_flag();
    uint8_t result = registers.af.msb - r8 - carry;

    bool has_half_carry =
        Registers::subtraction_half_carry(registers.af.msb, r8) ||
        Registers::subtraction_half_carry(registers.af.msb, carry) ||
        Registers::subtraction_half_carry(registers.af.msb - carry, r8);

    bool has_carry =
        Registers::subtraction_carry(registers.af.msb, r8) ||
        Registers::subtraction_carry(registers.af.msb, carry) ||
        Registers::subtraction_carry(registers.af.msb - carry, r8);
    
    registers.set_zero_flag(result == 0);
    registers.set_sub_flag(1);
    registers.set_half_carry_flag(has_half_carry);
    registers.set_carry_flag(has_carry);

    registers.af.msb = result;
}

void CPU::AND_A_r8(uint8_t& r8)
{
    uint8_t result = registers.af.msb & r8;
    
    registers.set_zero_flag(result == 0);
    registers.set_sub_flag(0);
    registers.set_half_carry_flag(1);
    registers.set_carry_flag(0);

    registers.af.msb = result;
}

void CPU::STOP()
{
    // debug only
    exit(1);
}


void CPU::DI()
{
    // DI doesn't have the 1 cycle delay that EI has
    ime_scheduler = 0;
    imu.set_ime(false);
}

void CPU::EI()
{
    // EI enables IME the following cycle, not immediately.
    ime_scheduler = 1;
}

// LD [r16], imm8
void CPU::LD_r16mem_imm8(uint16_t& r16)
{
    uint8_t imm8 = mmu.bus_read(registers.pc++);
    mmu.bus_write(r16, imm8);
}

// INC [r16]
void CPU::INC_r16mem(uint16_t& r16mem)
{
    uint8_t value = mmu.bus_read(r16mem);
    INC_r8(value);
    mmu.bus_write(r16mem, value);
}

// DEC [r16]
void CPU::DEC_r16mem(uint16_t& r16mem)
{
    uint8_t value = mmu.bus_read(r16mem);
    DEC_r8(value);
    mmu.bus_write(r16mem, value);
}

// BIT b3, r8
void CPU::BIT_b3_r8(uint8_t bit, uint8_t &r8source)
{
    uint8_t mask = (1 << bit);
    bool set = !!(mask & r8source);
    registers.set_zero_flag(!set);
    registers.set_sub_flag(0);
    registers.set_half_carry_flag(1);
}
void CPU::BIT_b3_r16mem(uint8_t bit, uint16_t &r16mem)
{
    uint8_t value = mmu.bus_read(r16mem);
    uint8_t mask = (1 << bit);
    bool set = !!(mask & value);

    registers.set_zero_flag(!set);
    registers.set_sub_flag(0);
    registers.set_half_carry_flag(1);
}

void CPU::RES_b3_r8(uint8_t bit, uint8_t& r8)
{
    r8 &= ~(1 << bit);
}
void CPU::RES_b3_r16mem(uint8_t bit, uint16_t& r16mem)
{
    uint8_t value = mmu.bus_read(r16mem);
    value &= ~(1 << bit);
    mmu.bus_write(r16mem, value);
}

void CPU::SET_b3_r8(uint8_t bit, uint8_t& r8)
{
    r8 |= 1 << bit;
}

void CPU::SET_b3_r16mem(uint8_t bit, uint16_t &r16mem)
{
    uint8_t value = mmu.bus_read(r16mem);
    value |= 1 << bit;
    mmu.bus_write(r16mem, value);
}

// INC r8
void CPU::INC_r8(uint8_t& r8)
{
    uint8_t prev = r8++;
    registers.set_half_carry_flag(Registers::addition_half_carry(prev, 1));
    registers.set_sub_flag(0);
    registers.set_zero_flag(r8 == 0);
}

// DEC r8
void CPU::DEC_r8(uint8_t& r8)
{
    uint8_t prev = r8--;
    registers.set_half_carry_flag(Registers::subtraction_half_carry(prev, 1));
    registers.set_sub_flag(1);
    registers.set_zero_flag(r8 == 0);
}

// JR cc, imm8
void CPU::JR_cc_imm8(bool condition_met)
{
    int8_t e = (int8_t) mmu.bus_read(registers.pc++);
    if (condition_met)
    {
        registers.pc += e;
    }
}

// JP imm16
void CPU::JP_imm16()
{
    uint16_t nn = mmu.bus_read_word(registers.pc++);
    registers.pc++;
    registers.pc = nn;
}

// JP HL
void CPU::JP_HL()
{
    registers.pc = registers.hl.word;
}

// LD r8, r8
void CPU::LD_r8_r8(uint8_t& r8dest, uint8_t& r8source)
{
    r8dest = r8source;
}

// LD r8, imm8
void CPU::LD_r8_imm8(uint8_t& r8dest)
{
    uint8_t n = mmu.bus_read(registers.pc++);
    r8dest = n;
}

// RLCA
void CPU::RLCA()
{
    uint8_t msbit = registers.af.msb >> 7;
    registers.af.msb <<= 1;
    registers.af.msb |= msbit;

    registers.set_zero_flag(0);
    registers.set_sub_flag(0);
    registers.set_half_carry_flag(0);
    registers.set_carry_flag(msbit);
}

// RRCA
void CPU::RRCA()
{
    uint8_t lsbit = registers.af.msb & 1;
    registers.af.msb >>= 1;
    registers.af.msb |= (lsbit << 7);

    registers.set_zero_flag(0);
    registers.set_sub_flag(0);
    registers.set_half_carry_flag(0);
    registers.set_carry_flag(lsbit);
}

// RRA
void CPU::RRA()
{
    uint8_t a = registers.af.msb;
    const uint8_t lsbit = a & 1;
    const uint8_t carry = registers.get_carry_flag();
    a >>= 1;
    a |= carry << 7;
    registers.af.msb = a;

    registers.set_zero_flag(0);
    registers.set_sub_flag(0);
    registers.set_half_carry_flag(0);
    registers.set_carry_flag(lsbit);
}

// DAA
void CPU::DAA()
{
    uint8_t offset = 0x00;
    uint8_t a = registers.af.msb;

    bool half_carry = registers.get_half_carry_flag();
    bool carry = registers.get_carry_flag();
    bool sub = registers.get_sub_flag();

    bool needs_carry = false;

    if (half_carry || !sub && ((a & 0x0F) > 0x09))
    {
        offset |= 0x06;
    }

    if (carry || !sub && (a > 0x99))
    {
        offset |= 0x60;
        needs_carry = true;
    }

    if (sub)
    {
        a -= offset;
    }
    else
    {
        a += offset;
    }
    
    registers.set_zero_flag(a == 0);
    registers.set_half_carry_flag(0);
    registers.set_carry_flag(needs_carry);

    registers.af.msb = a;
}

// CPL
void CPU::CPL()
{
    registers.af.msb = ~registers.af.msb;
    registers.set_sub_flag(1);
    registers.set_half_carry_flag(1);
}

// SCF
void CPU::SCF()
{
    registers.set_sub_flag(0);
    registers.set_half_carry_flag(0);
    registers.set_carry_flag(1);
}

// CCF
void CPU::CCF()
{
    registers.set_sub_flag(0);
    registers.set_half_carry_flag(0);
    registers.set_carry_flag(!registers.get_carry_flag());
}

// JR imm8
void CPU::JR_imm8()
{
    int8_t e = (int8_t)mmu.bus_read(registers.pc++);
    registers.pc += e;
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
void CPU::LD_A_r16mem(uint16_t &r16source)
{
    uint8_t r16mem = mmu.bus_read(r16source);
    registers.af.msb = r16mem;
}

// LDH A, [C]
void CPU::LDH_A_C()
{
    uint8_t ffc = mmu.bus_read(0xFF00 + registers.bc.lsb);
    registers.af.msb = ffc;
}

// LDH A, [imm8]
void CPU::LDH_A_imm8()
{
    uint8_t imm8 = mmu.bus_read(registers.pc++);
    uint8_t ffimm8 = mmu.bus_read(0xFF00 + imm8);
    registers.af.msb = ffimm8;
}

// LDH [imm8], A
void CPU::LDH_imm8_A()
{
    uint8_t imm8 = mmu.bus_read(registers.pc++);
    mmu.bus_write(0xFF00 + imm8, registers.af.msb);
}

// LDH [C], A
void CPU::LDH_C_A()
{
    mmu.bus_write(0xFF00 + registers.bc.lsb, registers.af.msb);
}

// LD r16, imm16
void CPU::LD_r16_imm16(uint16_t& r16dest)
{
    uint16_t nn = mmu.bus_read_word(registers.pc);
    registers.pc += 2;
    r16dest = nn;
}

// LD [imm16], SP
void CPU::LD_imm16_SP()
{
    uint16_t imm16 = mmu.bus_read_word(registers.pc);
    registers.pc += 2;
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
    registers.set_half_carry_flag(Registers::addition_half_carry(registers.hl, r16));
    registers.set_carry_flag(Registers::addition_carry(registers.hl, r16));
    registers.hl = sum;
}

// LD [imm16], A
void CPU::LD_imm16_A()
{
    uint16_t imm16 = mmu.bus_read_word(registers.pc++);
    registers.pc++;
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
    uint16_t imm16 = mmu.bus_read_word(registers.pc++);
    registers.pc++;
    uint8_t n = mmu.bus_read(imm16);
    registers.af.msb = n;
}

// ADD SP, imm8
void CPU::ADD_SP_imm8()
{
    uint16_t sp = registers.sp;
    int8_t imm8 = (int8_t) mmu.bus_read(registers.pc++);

    bool has_half_carry = Registers::addition_half_carry(static_cast<uint8_t>(sp & 0xff), imm8);
    bool has_carry = Registers::addition_carry(static_cast<uint8_t>(sp & 0xff), imm8);

    registers.set_zero_flag(0);
    registers.set_sub_flag(0);
    registers.set_half_carry_flag(has_half_carry);
    registers.set_carry_flag(has_carry);
    registers.sp += imm8;
}

// LD HL, SP + imm8
void CPU::LD_HL_SP_imm8()
{
    uint16_t sp = registers.sp;
    int8_t imm8 = (int8_t) mmu.bus_read(registers.pc++);

    bool has_half_carry = Registers::addition_half_carry(static_cast<uint8_t>(sp & 0xff), imm8);
    bool has_carry = Registers::addition_carry(static_cast<uint8_t>(sp & 0xff), imm8);

    registers.hl = registers.sp + imm8;

    registers.set_zero_flag(0);
    registers.set_sub_flag(0);
    registers.set_half_carry_flag(has_half_carry);
    registers.set_carry_flag(has_carry);
}

// LD SP, HL
void CPU::LD_SP_HL()
{
    registers.sp = registers.hl.word;
}

// PUSH r16stk
void CPU::PUSH_r16stk(uint16_t& r16source)
{
    registers.sp -= 2;
    mmu.bus_write_word(registers.sp, r16source);
}

// RLC r8
void CPU::RLC_r8(uint8_t& r8)
{
    uint8_t msbit = r8 >> 7;
    r8 <<= 1;
    r8 |= msbit;

    registers.set_zero_flag(r8 == 0);
    registers.set_sub_flag(0);
    registers.set_half_carry_flag(0);
    registers.set_carry_flag(msbit);
}
void CPU::RLC_r16mem(uint16_t& r16mem)
{
    uint8_t value = mmu.bus_read(r16mem);
    uint8_t msbit = value >> 7;
    value <<= 1;
    value |= msbit;
    mmu.bus_write(r16mem, value);

    registers.set_zero_flag(value == 0);
    registers.set_sub_flag(0);
    registers.set_half_carry_flag(0);
    registers.set_carry_flag(msbit);
}

void CPU::RRC_r8(uint8_t& r8)
{
    uint8_t lsbit = r8 & 1;
    r8 >>= 1;
    r8 |= (lsbit << 7);

    registers.set_zero_flag(r8 == 0);
    registers.set_sub_flag(0);
    registers.set_half_carry_flag(0);
    registers.set_carry_flag(lsbit);
}
void CPU::RRC_r16mem(uint16_t& r16mem)
{
    uint8_t value = mmu.bus_read(r16mem);
    uint8_t lsbit = value & 1;
    value >>= 1;
    value |= (lsbit << 7);
    mmu.bus_write(r16mem, value);

    registers.set_zero_flag(value == 0);
    registers.set_sub_flag(0);
    registers.set_half_carry_flag(0);
    registers.set_carry_flag(lsbit);
}

void CPU::RL_r8(uint8_t& r8)
{
    const uint8_t carry = registers.get_carry_flag();
    const uint8_t msbit = r8 >> 7;
    r8 <<= 1;
    r8 |= carry;

    registers.set_zero_flag(r8 == 0);
    registers.set_sub_flag(0);
    registers.set_half_carry_flag(0);
    registers.set_carry_flag(msbit);
}
// RL [r16mem]
void CPU::RL_r16mem(uint16_t& r16mem)
{
    uint8_t value = mmu.bus_read(r16mem);
    const uint8_t carry = registers.get_carry_flag();
    const uint8_t msbit = value >> 7;
    value <<= 1;
    value |= carry;
    mmu.bus_write(r16mem, value);

    registers.set_zero_flag(value == 0);
    registers.set_sub_flag(0);
    registers.set_half_carry_flag(0);
    registers.set_carry_flag(msbit);
}

void CPU::RR_r8(uint8_t& r8)
{
    const uint8_t lsbit = r8 & 1;
    const uint8_t carry = registers.get_carry_flag();
    r8 >>= 1;
    r8 |= carry << 7;

    registers.set_zero_flag(r8 == 0);
    registers.set_sub_flag(0);
    registers.set_half_carry_flag(0);
    registers.set_carry_flag(lsbit);
}
void CPU::RR_r16mem(uint16_t& r16mem)
{
    uint8_t value = mmu.bus_read(r16mem);
    const uint8_t lsbit = value & 1;
    const uint8_t carry = registers.get_carry_flag();
    value >>= 1;
    value |= carry << 7;
    mmu.bus_write(r16mem, value);

    registers.set_zero_flag(value == 0);
    registers.set_sub_flag(0);
    registers.set_half_carry_flag(0);
    registers.set_carry_flag(lsbit);
}

void CPU::SLA_r8(uint8_t& r8)
{
    uint8_t msbit = r8 >> 7;
    r8 <<= 1;

    registers.set_zero_flag(r8 == 0);
    registers.set_sub_flag(0);
    registers.set_half_carry_flag(0);
    registers.set_carry_flag(msbit);
}
void CPU::SLA_r16mem(uint16_t& r16mem)
{
    uint8_t value = mmu.bus_read(r16mem);
    uint8_t msbit = value >> 7;
    value <<= 1;
    mmu.bus_write(r16mem, value);

    registers.set_zero_flag(value == 0);
    registers.set_sub_flag(0);
    registers.set_half_carry_flag(0);
    registers.set_carry_flag(msbit);
}

void CPU::SRA_r8(uint8_t& r8)
{
    uint8_t lsbit = r8 & 1;
    uint8_t msbit = r8 & 0x80;
    r8 >>= 1;
    r8 |= msbit;
    registers.set_zero_flag(r8 == 0);
    registers.set_sub_flag(0);
    registers.set_half_carry_flag(0);
    registers.set_carry_flag(lsbit);
}
void CPU::SRA_r16mem(uint16_t& r16mem)
{
    uint8_t value = mmu.bus_read(r16mem);
    uint8_t lsbit = value & 1;
    uint8_t msbit = value & 0x80;
    value >>= 1;
    value |= msbit;
    mmu.bus_write(r16mem, value);

    registers.set_zero_flag(value == 0);
    registers.set_sub_flag(0);
    registers.set_half_carry_flag(0);
    registers.set_carry_flag(lsbit);
}

void CPU::SWAP_r8(uint8_t& r8)
{
    uint8_t lsnibble = r8 & 0x0F;
    uint8_t msnibble = r8 & 0xF0;
    r8 = (msnibble >> 4) | (lsnibble << 4);

    registers.set_zero_flag(r8 == 0);
    registers.set_sub_flag(0);
    registers.set_half_carry_flag(0);
    registers.set_carry_flag(0);
}
void CPU::SWAP_r16mem(uint16_t& r16mem)
{
    uint8_t value = mmu.bus_read(r16mem);
    uint8_t lsnibble = value & 0x0F;
    uint8_t msnibble = value & 0xF0;
    value = (msnibble >> 4) | (lsnibble << 4);
    mmu.bus_write(r16mem, value);

    registers.set_zero_flag(value == 0);
    registers.set_sub_flag(0);
    registers.set_half_carry_flag(0);
    registers.set_carry_flag(0);
}

void CPU::SRL_r8(uint8_t& r8)
{
    uint8_t lsbit = r8 & 1;
    r8 >>= 1;
    registers.set_zero_flag(r8 == 0);
    registers.set_sub_flag(0);
    registers.set_half_carry_flag(0);
    registers.set_carry_flag(lsbit);
}
void CPU::SRL_r16mem(uint16_t& r16mem)
{
    uint8_t value = mmu.bus_read(r16mem);
    uint8_t lsbit = value & 1;
    value >>= 1;
    mmu.bus_write(r16mem, value);

    registers.set_zero_flag(value == 0);
    registers.set_sub_flag(0);
    registers.set_half_carry_flag(0);
    registers.set_carry_flag(lsbit);
}

void CPU::POP_r16stk(uint16_t& r16dest)
{
    uint16_t data = mmu.bus_read_word(registers.sp);
    registers.sp += 2;

    r16dest = data;
}

void CPU::XOR_A_r8(uint8_t& r8dest)
{
    registers.af.msb ^= r8dest;
    registers.set_zero_flag(registers.af.msb == 0);
    registers.set_sub_flag(0);
    registers.set_carry_flag(0);
    registers.set_half_carry_flag(0);
}

void CPU::OR_A_r8(uint8_t& r8)
{
    uint8_t result = registers.af.msb | r8;
    registers.set_zero_flag(result == 0);
    registers.set_sub_flag(0);
    registers.set_half_carry_flag(0);
    registers.set_carry_flag(0);
    registers.af.msb = result;
}

void CPU::CP_A_r8(uint8_t& r8)
{
    uint8_t result = registers.af.msb - r8;
    registers.set_zero_flag(result == 0);
    registers.set_sub_flag(1);
    registers.set_half_carry_flag(Registers::subtraction_half_carry(registers.af.msb, r8));
    registers.set_carry_flag(Registers::subtraction_carry(registers.af.msb, r8));
}

void CPU::ADD_A_imm8()
{
    uint8_t imm8 = mmu.bus_read(registers.pc++);
    uint8_t result = registers.af.msb + imm8;
    registers.set_zero_flag(result == 0);
    registers.set_sub_flag(0);
    registers.set_half_carry_flag(Registers::addition_half_carry(registers.af.msb, imm8));
    registers.set_carry_flag(Registers::addition_carry(registers.af.msb, imm8));
    registers.af.msb = result;
}

void CPU::ADC_A_imm8()
{
    uint8_t imm8 = mmu.bus_read(registers.pc++);
    uint8_t carry = (uint8_t)registers.get_carry_flag();
    uint8_t result = registers.af.msb + imm8 + carry;

    bool has_half_carry =
        Registers::addition_half_carry(registers.af.msb, imm8) ||
        Registers::addition_half_carry(registers.af.msb, carry) ||
        Registers::addition_half_carry(registers.af.msb + carry, imm8);

    bool has_carry =
        Registers::addition_carry(registers.af.msb, imm8) ||
        Registers::addition_carry(registers.af.msb, carry) ||
        Registers::addition_carry(registers.af.msb + carry, imm8);

    registers.set_zero_flag(result == 0);
    registers.set_sub_flag(0);
    registers.set_half_carry_flag(has_half_carry);
    registers.set_carry_flag(has_carry);
    registers.af.msb = result;
}

void CPU::SUB_A_imm8()
{
    uint8_t imm8 = mmu.bus_read(registers.pc++);
    uint8_t result = registers.af.msb - imm8;
    registers.set_zero_flag(result == 0);
    registers.set_sub_flag(1);
    registers.set_half_carry_flag(Registers::subtraction_half_carry(registers.af.msb, imm8));
    registers.set_carry_flag(Registers::subtraction_carry(registers.af.msb, imm8));
    registers.af.msb = result;
}

void CPU::SBC_A_imm8()
{
    uint8_t imm8 = mmu.bus_read(registers.pc++);
    uint8_t carry = registers.get_carry_flag();
    uint8_t result = registers.af.msb - imm8 - carry;

    bool has_half_carry =
        Registers::subtraction_half_carry(registers.af.msb, imm8) ||
        Registers::subtraction_half_carry(registers.af.msb, carry) ||
        Registers::subtraction_half_carry(registers.af.msb - carry, imm8);

    bool has_carry =
        Registers::subtraction_carry(registers.af.msb, imm8) ||
        Registers::subtraction_carry(registers.af.msb, carry) ||
        Registers::subtraction_carry(registers.af.msb - carry, imm8);

    registers.set_zero_flag(result == 0);
    registers.set_sub_flag(1);
    registers.set_half_carry_flag(has_half_carry);
    registers.set_carry_flag(has_carry);
    registers.af.msb = result;
}

void CPU::AND_A_imm8()
{
    uint8_t imm8 = mmu.bus_read(registers.pc++);
    uint8_t result = registers.af.msb & imm8;
    registers.set_zero_flag(result == 0);
    registers.set_sub_flag(0);
    registers.set_half_carry_flag(1);
    registers.set_carry_flag(0);
    registers.af.msb = result;
}

void CPU::XOR_A_imm8()
{
    uint8_t imm8 = mmu.bus_read(registers.pc++);
    uint8_t result = registers.af.msb ^ imm8;
    registers.set_zero_flag(result == 0);
    registers.set_sub_flag(0);
    registers.set_half_carry_flag(0);
    registers.set_carry_flag(0);
    registers.af.msb = result;
}

void CPU::OR_A_imm8()
{
    uint8_t imm8 = mmu.bus_read(registers.pc++);
    uint8_t result = registers.af.msb | imm8;
    registers.set_zero_flag(result == 0);
    registers.set_sub_flag(0);
    registers.set_half_carry_flag(0);
    registers.set_carry_flag(0);
    registers.af.msb = result;
}

void CPU::CP_A_imm8()
{
    uint8_t imm8 = mmu.bus_read(registers.pc++);
    uint8_t result = registers.af.msb - imm8;
    registers.set_zero_flag(result == 0);
    registers.set_sub_flag(1);
    registers.set_half_carry_flag(Registers::subtraction_half_carry(registers.af.msb, imm8));
    registers.set_carry_flag(Registers::subtraction_carry(registers.af.msb, imm8));
}

// LD r8, [r16]
void CPU::LD_r8_r16mem(uint8_t& r8, uint16_t& r16mem)
{
    r8 = mmu.bus_read(r16mem);
}

// LD [r16], r8
void CPU::LD_r16mem_r8(uint16_t& r16mem, uint8_t& r8)
{
    mmu.bus_write(r16mem, r8);
}

// ADD A, [r16mem]
void CPU::ADD_A_r16mem(uint16_t& r16mem)
{
    uint8_t data = mmu.bus_read(r16mem);
    ADD_A_r8(data);
}

void CPU::ADC_A_r16mem(uint16_t& r16mem)
{
    uint8_t data = mmu.bus_read(r16mem);
    ADC_A_r8(data);
}

void CPU::SUB_A_r16mem(uint16_t& r16mem)
{
    uint8_t data = mmu.bus_read(r16mem);
    SUB_A_r8(data);
}

void CPU::SBC_A_r16mem(uint16_t& r16mem)
{
    uint8_t data = mmu.bus_read(r16mem);
    SBC_A_r8(data);
}

void CPU::AND_A_r16mem(uint16_t& r16mem)
{
    uint8_t data = mmu.bus_read(r16mem);
    AND_A_r8(data);
}

void CPU::XOR_A_r16mem(uint16_t& r16mem)
{
    uint8_t data = mmu.bus_read(r16mem);
    XOR_A_r8(data);
}

void CPU::OR_A_r16mem(uint16_t& r16mem)
{
    uint8_t data = mmu.bus_read(r16mem);
    OR_A_r8(data);
}

void CPU::CP_A_r16mem(uint16_t& r16mem)
{
    uint8_t data = mmu.bus_read(r16mem);
    CP_A_r8(data);
}