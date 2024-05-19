#pragma once
#include <cstdint>

union Register
{
    uint16_t word;
    struct
    {
        uint8_t lsb;
        uint8_t msb;
    };

    // Operator overloads
    Register& operator=(const uint16_t& value)
    {
        word = value;
        return *this;
    }

    operator uint16_t() const
    {
        return word;
    }
};

class Registers
{
public:
    Register af, bc, de, hl;
    uint16_t sp, pc;

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

    static bool addition_carry(uint8_t, uint8_t);
    static bool subtraction_carry(uint8_t, uint8_t);
    static bool addition_half_carry(uint8_t, uint8_t);
    static bool subtraction_half_carry(uint8_t, uint8_t);

    static bool addition_carry(uint16_t, uint16_t);
    static bool subtraction_carry(uint16_t, uint16_t);
    static bool addition_half_carry(uint16_t, uint16_t);
    static bool subtraction_half_carry(uint16_t, uint16_t);

    // Debug functions
    void print_registers();
};