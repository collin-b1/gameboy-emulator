#include "main.h"

const unsigned HEADER_START = 0x100;
const unsigned HEADER_END = 0x14F;

struct Metadata
{
	// We are using arrays of unsigned chars for raw byte data fields
	unsigned char entrypoint[4];              // 0x100-0x103
	unsigned char nintendo_logo[48];          // 0x104-0x133
	char title[15];                           // 0x134-0x142
	unsigned char cgb;                        // 0x143
	unsigned short new_licensee_code;         // 0x144-0x145
	unsigned char sgb;                        // 0x146
	unsigned char cartridge_type;             // 0x147
	unsigned char rom_size;                   // 0x148
	unsigned char ram_size;                   // 0x149
	unsigned char destination_code;           // 0x14A
	unsigned char old_licensee_code;          // 0x14B
	unsigned char mask_rom_version;           // 0x14C
	unsigned char header_checksum;            // 0x14D
	unsigned short global_checksum;           // 0x14E-0x14F
};

struct Operand
{
	bool immediate;
	char name[4];
	unsigned char bytes;
	unsigned short value;
};

struct Opcode
{
	unsigned char opcode;
	char name[4];
	unsigned char bytes;
	unsigned char cycles;
	Operand operands[2];
};

struct Instruction
{
	unsigned short address;
	Opcode opcode;
};

int main()
{
	std::ifstream rom;
	rom.open(std::string(ROMS_DIRECTORY) + "/snake.gb", std::ios::binary);
	rom.seekg(HEADER_START);

	Metadata metadata{};
	rom.read(reinterpret_cast<char*>(&metadata), sizeof(Metadata));

	std::cout << "Title: " << metadata.title << std::endl;
	std::cout << "CGB: " << static_cast<int>(metadata.cgb) << std::endl;
	std::cout << "SGB: " << static_cast<int>(metadata.sgb) << std::endl;
	std::cout << "New licensee code: " << metadata.new_licensee_code << std::endl;
	std::cout << "Cartridge type: " << static_cast<int>(metadata.cartridge_type) << std::endl;
	std::cout << "ROM size: " << static_cast<int>(metadata.rom_size) << std::endl;
	std::cout << "RAM size: " << static_cast<int>(metadata.ram_size) << std::endl;
	std::cout << "Destination code: " << static_cast<int>(metadata.destination_code) << std::endl;
	std::cout << "Old licensee code: " << static_cast<int>(metadata.old_licensee_code) << std::endl;
	std::cout << "Mask ROM version: " << static_cast<int>(metadata.mask_rom_version) << std::endl;
	std::cout << "Header checksum: " << static_cast<int>(metadata.header_checksum) << std::endl;
	std::cout << "Global checksum: " << metadata.global_checksum << std::endl;

	return 0;
}