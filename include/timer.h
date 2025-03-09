#pragma once

#include <cstdint>
#include "memory.h"
#include "interrupt.h"

class Timer : public IMemory
{
public:
    Timer(InterruptManager&);
    uint8_t read(uint16_t) const override;
    void write(uint16_t, uint8_t) override;
    void tick();
private:
    InterruptManager& imu;

    // Divider Register
    // - Incremented at 16384Hz (~16779Hz on SGB).
    // - Writing any value resets to 0x00.
    // - STOP instruction also resets, and doesn't start again until stop mode ends.
    uint8_t div;

    // Timer Counter
    // - Incremented at speed specified by TAC.
    // - When value overflows (past 0xFF), resets to value specified by TMA and interrupt 
    //   is requested.
    uint8_t tima;

    // Timer Modulo
    // - [0xFF]: Interrupt is requested at clock frequency in TAC (every increment is overflow).
    //   [0xFE]: Interrupt requested every two increments, clocked doubled.
    //   [0xFD]: Interrupt requested every three increments, clock tripled.
    //   [etc.]: ...
    // - If TMA write happens on same M-cycle as TMA is transfered to TIMA due to 
    //   timer overflow, old value is transfered to TIMA.
    uint8_t tma;

    // Timer Control
    // - Bits 0-1: Clock Select. Frequency at which TIMA is incremented.
    // - Bit 2: Whether TIMA is incremented. DIV is always incremented regardless of this bit.
    // - Bits 3-7: Unused.
    uint8_t tac;
    
};