#pragma once

#include "definitions.h"
#include "interrupt.h"
#include "memory.h"
#include "system.h"
#include <cstdint>
#include <string>

class Timer final : public IMemory, public ITickableSystem
{
public:
    explicit Timer(InterruptManager &);
    [[nodiscard]] u8 read(u16) const override;
    void write(u16, u8) override;
    void tick(u16) override;
    [[nodiscard]] std::string get_state() const;
    void stop_div(bool);

private:
    u64 _cycles;

    // DIV stops when CPU is in stopped mode
    bool _div_stopped;

    InterruptManager &imu;

    // Divider Register
    // - Incremented at 16384Hz (~16779Hz on SGB).
    // - Writing any value resets to 0x00.
    // - STOP instruction also resets, and doesn't start again until stop mode ends.
    u16 div;

    // Timer Counter
    // - Incremented at speed specified by TAC.
    // - When value overflows (past 0xFF), resets to value specified by TMA and interrupt
    //   is requested.
    u8 tima;

    // Timer Modulo
    // - [0xFF]: Interrupt is requested at clock frequency in TAC (every increment is overflow).
    //   [0xFE]: Interrupt requested every two increments, clocked doubled.
    //   [0xFD]: Interrupt requested every three increments, clock tripled.
    //   [etc.]: ...
    // - If TMA write happens on same M-cycle as TMA is transfered to TIMA due to
    //   timer overflow, old value is transfered to TIMA.
    u8 tma;

    // Timer Control
    // - Bits 0-1: Clock Select. Frequency at which TIMA is incremented.
    // - Bit 2: Whether TIMA is incremented. DIV is always incremented regardless of this bit.
    // - Bits 3-7: Unused.
    u8 tac;
};