#pragma once

#include <cstdint>

class ITickableSystem
{
public:
    virtual ~ITickableSystem() = default;

    virtual void tick(uint16_t) = 0;
    [[nodiscard]] uint64_t get_cycles() const {
        return this->_cycles;
    }
private:
    uint64_t _cycles{};
};