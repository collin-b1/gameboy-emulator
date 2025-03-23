#pragma once

#include <cstdint>
#include "memory.h"

class Serial final : public IMemory
{
public:
    Serial();
    [[nodiscard]] uint8_t read(uint16_t) const override;
    void write(uint16_t, uint8_t) override;

private:
    uint8_t sb, sc;
};