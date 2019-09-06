#pragma once
#include "initializer_list"
#include "string"
#include "vector"
#include "mutex"

using uint = unsigned int;

class MemRWer
{
public:
	MemRWer();
	~MemRWer();
	bool open_by_window(void* hwnd);

	void readData(void* lpAddress, int lenth, uint8_t* buffer);
	std::wstring readWstring(uint addr);
	std::string readString(uint addr);
	void writeWstring(uint addr, std::wstring str);
	int decryption(uint addr);
	void writeData(void* lpAddress, int lenth, uint8_t * buffer);
	void runRemoteThread(std::vector<uint8_t> opcode);
	template<class T>
	T read(uint lpBaseAddress) {
		T buffer;
		readData(reinterpret_cast<void*>(lpBaseAddress), sizeof(T), static_cast<uint8_t*>(&buffer));
		return buffer;
	}

	template<class T>
	T read(uint lpBaseAddress, std::initializer_list<uint> offset) {
		uint result = read<uint>(lpBaseAddress);
		for (auto i = 0; i < offset.size() - 1; i++) {
			result = read<uint>(result + offset.begin()[i]);
		}
		return read<T>((uint)result + (uint)offset.end()[-1]);
	}

	template<class T>
	void write(uint lpBaseAddress, T data) {
		writeData((void*)lpBaseAddress, sizeof(T), (unsigned char**)&data);
	}

	template<class T>
	T write(uint lpBaseAddress, std::initializer_list<uint> offset, T data) {
		int result;
		result = read<uint>(lpBaseAddress);
		for (int i = 0; i < offset.size() - 1; i++) {
			result = read<uint>(result + offset.begin()[i]);
		}
		return write<T>((uint)result + (uint)offset.end()[-1], data);
	}

private:
	void* hProcess;
	void* hwnd;
};

