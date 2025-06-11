#pragma once
#include "definitions.h"

union Register {
    u16 word;
    struct
    {
        u8 lsb;
        u8 msb;
    };

    // Operator overloads
    Register &operator=(const u16 &value)
    {
        word = value;
        return *this;
    }

    operator u16() const
    {
        return word;
    }
};

class Registers
{
public:
    Register af, bc, de, hl;

    // SP and PC can be regular u16 because MSB and LSB wont be accessed directly
    u16 sp, pc;

    Registers();

    // Convenience functions for modifying and accessing individual flag values,
    // same as doing bit masking on the F register.
    bool get_zero_flag();
    bool get_sub_flag();
    bool get_half_carry_flag();
    bool get_carry_flag();
    void set_zero_flag(bool);
    void set_sub_flag(bool);
    void set_half_carry_flag(bool);
    void set_carry_flag(bool);

    static bool addition_carry(u8, u8);
    static bool subtraction_carry(u8, u8);
    static bool addition_half_carry(u8, u8);
    static bool subtraction_half_carry(u8, u8);

    static bool addition_carry(u16, u16);
    static bool subtraction_carry(u16, u16);
    static bool addition_half_carry(u16, u16);
    static bool subtraction_half_carry(u16, u16);

    // Debug functions
    void print_registers();
};