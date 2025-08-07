#pragma once

#include "memory/memory.h"
#include <algorithm>
#include <vector>

class IMBC : public IMemory
{
public:
    virtual ~IMBC() = default;
};

template <size_t ROM_SIZE, size_t RAM_SIZE> class MBC : public IMBC
{
public:
    MBC(const std::vector<u8> &rom_data, bool has_ram, bool has_battery) : has_ram(has_ram), has_battery(has_battery)
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
        if (addr >= 0xA000 && addr <= 0xBFFF)
        {
            return read_ram(addr);
        }

        return 0xFF;
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

    virtual u8 read_ram(u16 addr) const
    {
        if (has_ram && ram_enabled)
        {
            return ram.at(((addr - 0xA000) + (ram_bank_select * 0x2000)) % RAM_SIZE);
        }

        return 0xFF;
    }

    virtual void handle_bank_switch(u16 addr, u8 data) = 0;

    virtual void write_ram(u16 addr, u8 data)
    {
        if (has_ram && ram_enabled && addr >= 0xA000 && addr <= 0xBFFF)
        {
            ram[((addr - 0xA000) + (ram_bank_select * 0x2000)) % RAM_SIZE] = data;
        }
    };

protected:
    bool has_ram;
    bool has_battery;

    u8 rom_bank_select = 0;
    u8 ram_bank_select = 0;
    bool ram_enabled = false;
    std::array<uint8_t, ROM_SIZE> rom;
    std::array<uint8_t, RAM_SIZE> ram;
};