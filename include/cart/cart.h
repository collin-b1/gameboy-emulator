#pragma once
#include <array>
#include <cstdint>
#include <fstream>
#include <string>

#include "definitions.h"
#include "memory/memory.h"

constexpr u16 BOOT_ROM_SIZE = 0x100;

constexpr u16 ROM_START = 0x0000;
constexpr u16 ROM_END = 0x7FFF;

constexpr u16 HEADER_START = 0x100;
constexpr u16 HEADER_END = 0x14F;

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
    u8 entrypoint[4];     // 0x100-0x103
    u8 nintendo_logo[48]; // 0x104-0x133

    // TODO: Look into if compiler padding affects this
    union // 0x134-0x143
    {
        u8 title_cgb[16];
        struct
        {
            u8 title_only[15];
            u8 cgb;
        };
    } title;
    u16 new_licensee_code; // 0x144-0x145
    u8 sgb;                // 0x146
    u8 cartridge_type;     // 0x147
    u8 rom_size;           // 0x148
    u8 ram_size;           // 0x149
    u8 destination_code;   // 0x14A
    u8 old_licensee_code;  // 0x14B
    u8 mask_rom_version;   // 0x14C
    u8 header_checksum;    // 0x14D
    u16 global_checksum;   // 0x14E-0x14F
};

class Cart final : public IMemory
{
public:
    Cart();

    bool load_rom(const std::string &);
    bool load_boot_rom(const std::string &);
    bool load_header();

    bool is_boot_rom_disabled() const;
    [[nodiscard]] u8 read(u16) const override;
    void write(u16, u8) override;
    void print_headers() const;

    // Load buffer from file to an std::array
    template <typename T, size_t N> bool load_buffer(const std::string &path, std::array<T, N> &buffer)
    {
        std::ifstream rom_file;
        rom_file.open(path, std::ios::binary);

        if (!rom_file.is_open())
        {
            return false;
        }

        rom_file.seekg(0, std::ios::end);
        rom_file.seekg(0, std::ios::beg);
        rom_file.read(reinterpret_cast<char *>(buffer.data()), buffer.size());
        rom_file.close();

        return true;
    }

private:
    std::string name;
    CartHeaders headers;
    std::array<u8, ROM_END - ROM_START + 1> rom;
    std::array<u8, BOOT_ROM_SIZE> boot_rom;
    u8 bank;
    bool boot_rom_disabled;
};