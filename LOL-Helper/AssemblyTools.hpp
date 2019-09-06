#pragma once
#include <vector>

class AssemblyTools
{
public:
	AssemblyTools() {
		code_.clear();
	}
	
	template<class T>
	void write(T data) {
		for (int i = 0; i < sizeof(data); i++) {
			code_.push_back(((uint8_t*)&data)[i]);
		}
	}

	void clear() { code_.clear(); }

	void pushad() {
		write<uint8_t>(0x60);
	}

	void popad() {
		write<uint8_t>(0x61);
	}

	void mov_eax(int value) {
		write<uint8_t>(0xB8);
		write<int>(value);
	}


	void mov_eax_ptr_esi() {
		write<uint8_t>(0x8B);
		write<uint8_t>(0x06);
	}

	void mov_eax_edx() {
		write<uint8_t>(0x8B);
		write<uint8_t>(0xC2);
	}

	void xor_eax_eax() {
		write<uint8_t>(0x33);
		write<uint8_t>(0xC0);
	}

	void add_eax(int value) {
		write<uint8_t>(0x05);
		write<int>(value);
	}

	void mov_ecx_eax() {
		write<uint8_t>(0x8B);
		write<uint8_t>(0xC8);
	}

	void mov_eax_ptr_eax_addx(int value) {
		write<uint8_t>(0x8B);
		write<uint8_t>(0x80);
		write<int>(value);
	}

	void mov_eax_ptr_edi_addx(int value) {
		write<uint8_t>(0x8B);
		write<uint8_t>(0x87);
		write<int>(value);
	}

	void mov_ebx(int value) {
		write<uint8_t>(0xBB);
		write<int>(value);
	}

	void mov_ebx_ptr_eax() {
		write<uint8_t>(0x8B);
		write<uint8_t>(0x18);
	}

	void mov_ecx(int value) {
		write<uint8_t>(0xB9);
		write<int>(value);
	}

	void mov_edx(int value) {
		write<uint8_t>(0xBA);
		write<int>(value);
	}

	void mov_edx_ptr_ecx_addx(int value) {
		write<uint8_t>(0x8B);
		write<uint8_t>(0x91);
		write<int>(value);
	}

	void mov_esi(int value) {
		write<uint8_t>(0xBE);
		write<int>(value);
	}

	void mov_ecx_ptr(int value) {
		write<uint8_t>(0x8B);
		write<uint8_t>(0x0D);
		write<int>(value);
	}

	void mov_ecx_ptr_ecx() {
		write<uint8_t>(0x8B);
		write<uint8_t>(0x09);
	}

	void mov_ecx_ptr_ecx_addx(int value) {
		write<uint8_t>(0x8B);
		write<uint8_t>(0x89);
		write<int>(value);
	}

	void mov_esi_ecx() {
		write<uint8_t>(0x8B);
		write<uint8_t>(0xF1);
	}

	void mov_edi_eax() {
		write<uint8_t>(0x8B);
		write<uint8_t>(0xF8);
	}
	void mov_edi_esi() {
		write<uint8_t>(0x8B);
		write<uint8_t>(0xF7);
	}


	void mov_edi(int value) {
		write<uint8_t>(0xC7);
		write<uint8_t>(0xC7);
		write<int>(value);
	}
	void mov_esi_ptr_ebp_addx(int value) {
		write<uint8_t>(0x8B);
		write<uint8_t>(0xB5);
		write<int>(value);
	}

	void mov_ecx_esi() {
		write<uint8_t>(0x8B);
		write<uint8_t>(0xCE);
	}

	void push(int value) {
		write<uint8_t>(0x68);
		write<int>(value);
	}

	void mov_esp_ptr_addx(uint8_t addx, uint32_t value)
	{
		write<uint8_t>(0xC7);
		write<uint8_t>(0x44);
		write<uint8_t>(0x24);
		write<uint8_t>(addx);
		write<uint32_t>(value);
	}

	void push_eax() {
		write<uint8_t>(0x50);
	}

	void push_ptr(int value) {
		write<uint8_t>(0xFF);
		write<uint8_t>(0x35);
		write<int>(value);
	}

	void push_ecx() {
		write<uint8_t>(0x51);
	}

	void push_edx() {
		write<uint8_t>(0x52);
	}

	void mov_esi_eax() {
		write<uint8_t>(0x8B);
		write<uint8_t>(0xF0);
	}

	void call_eax() {
		write<uint8_t>(0xFF);
		write<uint8_t>(0xD0);
	}

	void mov_addr_eax(int addr) {

	}

	void add_esp(uint8_t b) {
		write<uint8_t>(0x83);
		write<uint8_t>(0xC4);
		write<uint8_t>(b);
	}

	void mov_xxx_eax(int address) {
		write<uint8_t>(0xA3);
		write<int>(address);
	}
	void cmp_eax_edi() {
		write<uint8_t>(0x3B);
		write<uint8_t>(0xC7);
	}
	void je(int bit) {
		if (bit <= 0x7F && bit >= -0x7F) {
			write<uint8_t>(0x75);
			write<uint8_t>(bit);
		}
		else {
			write<uint8_t>(0x0F);
			write<uint8_t>(0x85);
			write<uint32_t>(bit);
		}
	}
	void call_ebx() {
		write<uint8_t>(0xFF);
		write<uint8_t>(0xD3);
	}

	void call_edx() {
		write<uint8_t>(0xFF);
		write<uint8_t>(0xD2);
	}

	void call_edi() {
		write<uint8_t>(0xFF);
		write<uint8_t>(0xD7);
	}

	void ret() {
		write<uint8_t>(0xC3);
	}

	void ret(int value) {
		write<uint8_t>(0xC2);
		write<short>(value);
	}
	auto& getCode() {
		return code_;
	}
private:
	std::vector<uint8_t> code_;
};

