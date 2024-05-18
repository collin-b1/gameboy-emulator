#include "memory/cart.h"
#include <fstream>
#include <iostream>

Cart::Cart() : headers() {}
Cart::~Cart() {
	Cart::buffer.clear();
	Cart::buffer.shrink_to_fit();
	Cart::buffer.~vector();
}

uint8_t Cart::read(uint16_t addr)
{
	return buffer[addr];
}

void Cart::write(uint16_t addr, uint8_t data)
{
    buffer[addr] = data;
}

bool Cart::load_rom(const std::string& path)
{
	std::ifstream rom;
	rom.open(path, std::ios::binary);

	if (!rom.is_open())
	{
		std::cerr << "Error opening file" << std::endl;
		return false;
	}

	rom.seekg(0, std::ios::end);
	buffer.resize(rom.tellg());
	rom.seekg(0, std::ios::beg);
	rom.read(reinterpret_cast<char*>(buffer.data()), buffer.size());
	rom.close();

	return true;
}

bool Cart::load_header()
{
	headers = *reinterpret_cast<cart_headers*>(buffer.data() + HEADER_START);

	std::cout << "Title: " << headers.title << std::endl;
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

	uint8_t checksum = 0;
	for (uint16_t address = 0x0134; address <= 0x014C; address++)
	{
		checksum = checksum - buffer[address] - 1;
	}

	if (headers.header_checksum != checksum)
	{
		std::cerr << "Checksum does not match!" << std::endl;
		return false;
	}
	std::cout << "Checksum matches!" << std::endl;

	return true;
}