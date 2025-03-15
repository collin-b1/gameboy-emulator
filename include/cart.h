#pragma once
#include <string>
#include <array>
#include <cstdint>
#include "memory.h"

constexpr uint16_t BOOT_ROM_SIZE = 0x100;

constexpr uint16_t ROM_START = 0x0000;
constexpr uint16_t ROM_END = 0x7FFF;

constexpr uint16_t HEADER_START = 0x100;
constexpr uint16_t HEADER_END = 0x14F;

enum CartridgeType
{
    ROM_ONLY = 0x00,
    MBC1 = 0x01,
    MBC1_RAM = 0x02,
    MBC1_RAM_BATTERY = 0x03,
    MBC2 = 0x05,
    MBC2_BATTERY = 0x0,
    ROM_RAM = 0x08,
    ROM_RAM_BATTERY = 0x09,
    MMM01 = 0x0B,
    MMM01_RAM = 0x0C,
    MMM01_RAM_BATTERY = 0x0D,
    MBC3_TIMER_BATTERY = 0x0F,
    MBC3_TIMER_RAM_BATTERY = 0x10,
    MBC3 = 0x11,
    MBC3_RAM = 0x12,
    MBC3_RAM_BATTERY = 0x13,
    MBC5 = 0x19,
    MBC5_RAM = 0x1A,
    MBC_RAM_BATTERY = 0x1B,
    MBC5_RUMBLE = 0x1C,
    MBC5_RUMBLE_RAM = 0x1D,
    MBC5_RUMBLE_RAM_BATTERY = 0x1E,
    MBC6 = 0x20,
    MBC7_SENSOR_RUMBLE_RAM_BATTERY = 0x22,
    POCKET_CAMERA = 0xFC,
    BANDAI_TAMA5 = 0xFD,
    HuC3 = 0xFE,
    HuC1_RAM_BATTERY = 0xFF
};

struct CartHeaders
{
    uint8_t entrypoint[4];              // 0x100-0x103
    uint8_t nintendo_logo[48];          // 0x104-0x133

    // TODO: Look into if compiler padding affects this
    union                               // 0x134-0x143
    {
        uint8_t title_cgb[16];
        struct
        {
            uint8_t title_only[15];
            uint8_t cgb;
        };
    } title;
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

class Cart : public IMemory
{
public:
    Cart();

    bool load_rom(std::string&);
    bool load_boot_rom(std::string&);
    bool load_header();
    uint8_t read(uint16_t) const override;
    void write(uint16_t, uint8_t) override;
    void print_headers() const;

    // Load buffer from file to an std::array
    template <typename T, size_t N>
    bool load_buffer(std::string& path, std::array<T, N>& buffer)
    {
        std::ifstream rom_file;
        rom_file.open(path, std::ios::binary);

        if (!rom_file.is_open())
        {
            return false;
        }

        rom_file.seekg(0, std::ios::end);
        rom_file.seekg(0, std::ios::beg);
        rom_file.read(reinterpret_cast<char*>(buffer.data()), buffer.size());
        rom_file.close();

        return load_header();
    }

private:
    std::string name;
    CartHeaders headers;
    std::array<uint8_t, ROM_END - ROM_START + 1> rom;
    std::array<uint8_t, BOOT_ROM_SIZE> boot_rom;
    uint8_t bank;
};