#include "cart/cart.h"
#include "cart/mbc.h"
#include <iostream>

// No MBC: 32kb ROM and 8kb RAM
namespace MBCTypes
{
class NoMBC : public MBC<0x8000, 0x2000>
{
public:
    NoMBC(const std::vector<u8> &rom_data) : MBC(rom_data, false, false) {};

    void handle_bank_switch(u16 addr, u8 data) override
    {
        // Do nothing
    }

    void write_ram(u16 addr, u8 data) override
    {
        // Do nothing
    }
};

// MBC1: 512kb ROM and 32kb RAM
class MBC1 : public MBC<0x80000, 0x8000>
{
public:
    MBC1(const std::vector<u8> &rom_data, bool has_ram, bool has_battery) : MBC(rom_data, has_ram, has_battery) {};

    void handle_bank_switch(u16 addr, u8 data) override
    {
        if (addr <= 0x1FFF)
        {
            ram_enabled = (data & 0x0F) == 0xA;
        }

        // ROM bank
        else if (addr <= 0x3FFF)
        {
            // TODO: Make accurate to docs
            // https://gbdev.io/pandocs/MBC1.html#40005fff--ram-bank-number--or--upper-bits-of-rom-bank-number-write-only
            rom_bank_select = data & 0x1F;
        }

        // RAM bank OR upper bits of ROM bank number
        else if (addr <= 0x5FFF)
        {
            ram_bank_select = data & 0x3;
        }

        // Banking mode select
        // https://gbdev.io/pandocs/MBC1.html#60007fff--banking-mode-select-write-only
        else if (addr <= 0x7FFF)
        {
            ram_bank_select = data & 0x1;
        }
    }

    //    void write_ram(u16 addr, u8 data) override
    //    {
    //
    //    }

private:
    bool banking_mode;
};

class MBC3 : public MBC<0x100000, 0x8000>
{
public:
    MBC3(const std::vector<u8> &rom_data, bool has_ram, bool has_battery, bool has_rtc)
        : MBC(rom_data, has_ram, has_battery), has_rtc(has_rtc) {};

    void handle_bank_switch(u16 addr, u8 data) override
    {
        if (addr <= 0x1FFF)
        {
            ram_enabled = (data & 0x0F) == 0xA;
        }

        // ROM bank
        else if (addr <= 0x3FFF)
        {
            u8 rom_bank = data & 0x7F;
            rom_bank_select = (rom_bank == 0) ? 1 : rom_bank;
        }

        // RAM bank OR upper bits of ROM bank number
        else if (addr <= 0x5FFF)
        {
            ram_bank_select = data;
        }

        // Banking mode select
        // https://gbdev.io/pandocs/MBC1.html#60007fff--banking-mode-select-write-only
        else if (addr <= 0x7FFF)
        {
            // TODO: Implement latch clock for RTC
        }
    }

    u8 read_ram(u16 addr) const override
    {
        if (has_ram && ram_enabled)
        {
            if (ram_bank_select <= 0x03)
            {
                const size_t offset = (addr - 0xA000) + (ram_bank_select * 0x2000);
                if (offset < ram.size())
                {
                    return ram.at(offset);
                }
            }
            else if (has_rtc && ram_bank_select >= 0x08 && ram_bank_select <= 0x0C)
            {
                // TODO: Replace with actual RTC registers
                return 0xFF;
            }
        }

        return 0xFF;
    }

    void write_ram(u16 addr, u8 data) override
    {
        if (has_ram && ram_enabled && addr >= 0xA000 && addr <= 0xBFFF)
        {
            if (ram_bank_select <= 0x03) // 0x00 - 0x03 is used for RAM
            {
                const size_t offset = (addr - 0xA000) + (ram_bank_select * 0x2000);
                if (offset < ram.size())
                {
                    ram[offset] = data;
                }
            }
            else if (ram_bank_select >= 0x08) // 0x08 - 0x0C is used for RTC
            {
                // TODO: Implement RTC registers
            }
        }
    }

private:
    bool has_rtc;
};
} // namespace MBCTypes

Cart::Cart() : headers() /*, rom()*/, boot_rom(), boot_rom_disabled(false)
{
}

bool Cart::is_boot_rom_disabled() const
{
    return boot_rom_disabled;
}

u8 Cart::read(const u16 addr) const
{
    if (!boot_rom_disabled && addr < 0x0100)
    {
        return boot_rom.at(addr);
    }
    else if (addr == 0xFF50)
    {
        return 0;
    }

    // return rom.at(addr);
    return mbc->read(addr);
}

void Cart::write(const u16 addr, const u8 data)
{

    if (addr == 0xFF50)
    {
        boot_rom_disabled = data;
    }
    else
    {
        mbc->write(addr, data);
    }
}

bool Cart::load_boot_rom(const std::string &path)
{
    if (!path.empty())
    {
        const auto loaded = load_buffer(path, boot_rom);
        //        for (int i = 0x0104; i <= 0x0133; ++i)
        //        {
        //            std::cout << std::hex << std::uppercase << (int)rom[i] << ' ';
        //        }
        //        std::cout << "\n";
        return loaded;
    }
    else
    {
        return false;
    }
}

bool Cart::load_rom(const std::string &path)
{
    std::vector<u8> rom_data;

    if (load_buffer(path, rom_data))
    {
        return load_headers_and_mbc(rom_data);
    }
    else
    {
        return false;
    }
}

bool Cart::load_headers_and_mbc(const std::vector<u8> &rom_data)
{
    headers = *reinterpret_cast<const CartHeaders *>(rom_data.data() + HEADER_START);

    u8 checksum = 0;
    for (u16 address = 0x0134; address <= 0x014C; address++)
    {
        checksum = checksum - rom_data.at(address) - 1;
    }

    if (rom_data.at(0x014D) != checksum)
    {
        std::cerr << "Checksum does not match!" << std::endl;
        return false;
    }

    switch (headers.cartridge_type)
    {
    // ROM Only
    case CartridgeType::ROM_ONLY:
        mbc = std::make_unique<MBCTypes::NoMBC>(rom_data);
        break;

    // MBC1
    case CartridgeType::MBC1:
        mbc = std::make_unique<MBCTypes::MBC1>(rom_data, false, false);
        break;
    case CartridgeType::MBC1_RAM:
        mbc = std::make_unique<MBCTypes::MBC1>(rom_data, true, false);
        break;
    case CartridgeType::MBC1_RAM_BATTERY:
        mbc = std::make_unique<MBCTypes::MBC1>(rom_data, true, true);
        break;

    // MBC3
    case CartridgeType::MBC3_TIMER_BATTERY:
        mbc = std::make_unique<MBCTypes::MBC3>(rom_data, false, true, true);
        break;
    case CartridgeType::MBC3_TIMER_RAM_BATTERY:
        mbc = std::make_unique<MBCTypes::MBC3>(rom_data, true, true, true);
        break;
    case CartridgeType::MBC3:
        mbc = std::make_unique<MBCTypes::MBC3>(rom_data, false, false, false);
        break;
    case CartridgeType::MBC3_RAM:
        mbc = std::make_unique<MBCTypes::MBC3>(rom_data, true, false, false);
        break;
    case CartridgeType::MBC3_RAM_BATTERY:
        mbc = std::make_unique<MBCTypes::MBC3>(rom_data, true, true, false);
        break;

    default:
        std::cout << "Unsupported MBC type: 0x" << std::hex << static_cast<int>(headers.cartridge_type) << std::endl;
        exit(7);
    }

    return true;
}

void Cart::print_headers() const
{
    std::cout << "Title: " << headers.title.title_only << std::endl;
    std::cout << "CGB flag: 0x" << std::hex << static_cast<int>(headers.title.cgb) << std::endl;
    std::cout << "Entrypoint: 0x" << std::hex << static_cast<int>(headers.entrypoint[0]) << std::dec << " 0x"
              << std::hex << static_cast<int>(headers.entrypoint[1]) << std::dec << " 0x" << std::hex
              << static_cast<int>(headers.entrypoint[2]) << std::dec << " 0x" << std::hex
              << static_cast<int>(headers.entrypoint[3]) << std::dec << std::endl;
    std::cout << "New licensee code: " << headers.new_licensee_code << std::endl;
    std::cout << "Cartridge type: " << static_cast<int>(headers.cartridge_type) << std::endl;
    std::cout << "ROM size: " << static_cast<int>(headers.rom_size) << std::endl;
    std::cout << "RAM size: " << static_cast<int>(headers.ram_size) << std::endl;
    std::cout << "Destination code: " << static_cast<int>(headers.destination_code) << std::endl;
    std::cout << "Old licensee code: " << static_cast<int>(headers.old_licensee_code) << std::endl;
    std::cout << "Mask ROM version: " << static_cast<int>(headers.mask_rom_version) << std::endl;
}