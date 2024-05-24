#pragma once

#include <cstdint>

class Serial
{
public:
    Serial();

    uint8_t read(uint16_t);
    void write(uint16_t, uint8_t);
private:
    uint8_t sb, sc;
};