#pragma once

#include "cart/imbc.h"
#include <algorithm>
#include <vector>

template <size_t ROM_SIZE, size_t RAM_SIZE> class MBC : public IMBC
{
public:
    MBC(const std::vector<u8> &rom_data)
    {
        std::copy_n(rom_data.begin(), std::min(rom_data.size(), rom.size()), rom.begin());
    }

    u8 read(u16 addr) const override
    {
        // [0, 0x3FFF] is always used for ROM bank 0
        if (addr >= 0x0000 && addr <= 0x3FFF)
        {
            return rom.at(addr);
        }

        // ROM bank 01-NN
        if (addr >= 0x4000 && addr <= 0x7FFF)
        {
            const u8 rom_bank_mapped = rom_bank_select == 0 ? 1 : rom_bank_select;
            return rom.at(((addr - 0x4000) + (rom_bank_mapped * 0x4000)));
        }

        // https://gbdev.io/pandocs/MBCs.html#mbc-unmapped-ram-bank-access
        if (ram_enabled && addr >= 0xA000 && addr <= 0xBFFF)
        {
            return ram.at(((addr - 0xA000) + (ram_bank_select * 0x2000)) % RAM_SIZE);
        }

        return 0;
    }

    void write(u16 addr, u8 data) override
    {
        // ROM writes change banks in different ways depending on MBC
        if (addr <= 0x7FFF)
        {
            handle_bank_switch(addr, data);
        }

        // RAM
        if (addr >= 0xA000 && addr <= 0xBFFF)
        {
            write_ram(addr, data);
        }
    }

    virtual void handle_bank_switch(u16 addr, u8 data) = 0;

    virtual void write_ram(u16 addr, u8 data)
    {
        if (ram_enabled && addr >= 0xA000 && addr <= 0xBFFF)
        {
            ram[((addr - 0xA000) + (ram_bank_select * 0x2000)) % RAM_SIZE] = data;
        }
    };

protected:
    u8 rom_bank_select = 0;
    u8 ram_bank_select = 0;
    bool ram_enabled = false;
    std::array<uint8_t, ROM_SIZE> rom;
    std::array<uint8_t, RAM_SIZE> ram;
};