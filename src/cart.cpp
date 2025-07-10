#include "cart.h"
#include <iostream>

Cart::Cart() : headers(), rom(), boot_rom(), bank(0), boot_rom_disabled(false)
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

    return rom.at(addr);
}

void Cart::write(const u16 addr, const u8 data)
{

    if (addr == 0xFF50)
    {
        boot_rom_disabled = data;
    }
    else
    {
        /*std::cout << std::hex << "Invalid write to addr 0x" << addr << " with data 0x" << data << std::endl;
        exit(7);*/
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
    if (load_buffer(path, rom))
    {
        return load_header();
    }
    else
    {
        return false;
    }
}

bool Cart::load_header()
{
    headers = *reinterpret_cast<CartHeaders *>(rom.data() + HEADER_START);

    u8 checksum = 0;
    for (u16 address = 0x0134; address <= 0x014C; address++)
    {
        checksum = checksum - rom[address] - 1;
    }

    if (rom[0x014D] != checksum)
    {
        std::cerr << "Checksum does not match!" << std::endl;
        return false;
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