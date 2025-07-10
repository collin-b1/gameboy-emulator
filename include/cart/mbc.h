#pragma once

#include "memory/memory.h"

template <size_t ROM_SIZE, size_t RAM_SIZE> class MBC : public IMemory
{
private:
    std::array<uint8_t, ROM_SIZE> rom;
    std::array<uint8_t, RAM_SIZE> ram;
};