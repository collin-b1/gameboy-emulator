#pragma once

#define HEADER_START 0x100
#define HEADER_END 0x14F
#include <string>
#include <vector>
#include <cstdint>

struct cart_headers
{
    uint8_t entrypoint[4];              // 0x100-0x103
    uint8_t nintendo_logo[48];          // 0x104-0x133
    char title[15];                     // 0x134-0x142
    uint8_t cgb;                        // 0x143
    uint16_t new_licensee_code;         // 0x144-0x145
    uint8_t sgb;                        // 0x146
    uint8_t cartridge_type;             // 0x147
    uint8_t rom_size;                   // 0x148
    uint8_t ram_size;                   // 0x149
    uint8_t destination_code;           // 0x14A
    uint8_t old_licensee_code;          // 0x14B
    uint8_t mask_rom_version;           // 0x14C
    uint8_t header_checksum;            // 0x14D
    uint16_t global_checksum;           // 0x14E-0x14F
};

class Cart
{
public:
    Cart();
    ~Cart();

    bool load_rom(const std::string& path);
    bool load_header();
    uint8_t read(uint16_t addr);
    void write(uint16_t addr, uint8_t data);

private:
    std::string name;
    cart_headers headers;
    std::vector<uint8_t> buffer;
};