#include "cart.h"
#include <fstream>
#include <iostream>

Cart::Cart() : 
    headers(), 
    bank(0),
    boot_rom(0x100),
    rom(ROM_END - ROM_START + 1)
{}

uint8_t Cart::read(uint16_t addr) const
{
    if (addr < 0x00FF)
    {
        if (bank)
        {
            return rom[addr];
        }
        else
        {
            return boot_rom[addr];
        }
    }
    if (addr == 0xFF50)
    {
        return bank;
    }
    return rom.at(addr);
}

void Cart::write(uint16_t addr, uint8_t data)
{
    if (addr == 0xFF50)
    {
        bank = data;
    }
    else
    {
        exit(7);
    }
}

bool Cart::load_boot_rom(std::string& path)
{
    std::ifstream rom_file;
    rom_file.open(path, std::ios::binary);

    if (!rom_file.is_open())
    {
        std::cerr << "Error opening Boot ROM file" << std::endl;
        return false;
    }

    rom_file.seekg(0, std::ios::end);
    rom_file.seekg(0, std::ios::beg);
    rom_file.read(reinterpret_cast<char*>(boot_rom.data()), boot_rom.size());
    rom_file.close();

    return true;
}

bool Cart::load_rom(std::string &path)
{
    std::ifstream rom_file;
    rom_file.open(path, std::ios::binary);

    if (!rom_file.is_open())
    {
        std::cerr << "Error opening ROM file" << std::endl;
        return false;
    }

    rom_file.seekg(0, std::ios::end);
    rom_file.seekg(0, std::ios::beg);
    rom_file.read(reinterpret_cast<char*>(rom.data()), rom.size());
    rom_file.close();

    return load_header();
}

bool Cart::load_header()
{
    headers = *reinterpret_cast<CartHeaders*>(rom.data() + HEADER_START);

    uint8_t checksum = 0;
    for (uint16_t address = 0x0134; address <= 0x014C; address++)
    {
        checksum = checksum - rom[address] - 1;
    }

    if (headers.header_checksum != checksum)
    {
        std::cerr << "Checksum does not match!" << std::endl;
        return false;
    }

    return true;
}

void Cart::print_headers() const
{
    std::cout << "Title: " << headers.title.title_only << std::endl;
    std::cout << "CGB flag: 0x" << std::hex << (int)headers.title.cgb << std::endl;
    std::cout << "Entrypoint: 0x" <<
        std::hex << static_cast<int>(headers.entrypoint[0]) << std::dec << " 0x" <<
        std::hex << static_cast<int>(headers.entrypoint[1]) << std::dec << " 0x" <<
        std::hex << static_cast<int>(headers.entrypoint[2]) << std::dec << " 0x" <<
        std::hex << static_cast<int>(headers.entrypoint[3]) << std::dec <<
        std::endl;
    std::cout << "New licensee code: " << headers.new_licensee_code << std::endl;
    std::cout << "Cartridge type: " << static_cast<int>(headers.cartridge_type) << std::endl;
    std::cout << "ROM size: " << static_cast<int>(headers.rom_size) << std::endl;
    std::cout << "RAM size: " << static_cast<int>(headers.ram_size) << std::endl;
    std::cout << "Destination code: " << static_cast<int>(headers.destination_code) << std::endl;
    std::cout << "Old licensee code: " << static_cast<int>(headers.old_licensee_code) << std::endl;
    std::cout << "Mask ROM version: " << static_cast<int>(headers.mask_rom_version) << std::endl;
}