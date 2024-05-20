#pragma once
#include <vector>
#include "cart.h"

class MemoryBus
{
public:
    MemoryBus();
    ~MemoryBus();
    uint8_t bus_read(uint16_t);
    uint16_t bus_read_word(uint16_t);
    void bus_write(uint16_t addr, uint8_t data);
    void bus_write_word(uint16_t addr, uint16_t data);
    Cart& get_cart();
private:
    std::vector<uint8_t> io;
    std::vector<uint8_t> wram;
    std::vector<uint8_t> hram;
    Cart cart;
};