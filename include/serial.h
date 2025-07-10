#pragma once

#include "definitions.h"
#include "memory/memory.h"

class Serial final : public IMemory
{
public:
    Serial();
    [[nodiscard]] u8 read(u16) const override;
    void write(u16, u8) override;

private:
    u8 sb, sc;
};