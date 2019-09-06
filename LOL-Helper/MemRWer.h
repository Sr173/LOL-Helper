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
	void set_execute_code_memory(uint addr) { remote_addr_ = addr; }
	std::wstring readWstring(uint addr);
	std::string readString(uint addr);
	void writeWstring(uint addr, std::wstring str);
	int decryption(uint addr);
	int runRemoteThread(std::vector<uint8_t> opcode);
	auto get_game_handle() { return hProcess; }

	template <typename T1, typename  T2>
	void readData(T1 lpAddress, int lenth, T2 buffer)
	{
		readDataImpl((void*)lpAddress, lenth, (uint8_t*)buffer);
	};

	template <typename T1,typename  T2>
	void writeData(T1 lpAddress, int lenth, T2 buffer)
	{
		writeDataImpl((void*)lpAddress, lenth, (uint8_t*)buffer);
	};

	template<class T>
	T read(uint lpBaseAddress) {
		T buffer = 0;
		readData(reinterpret_cast<void*>(lpBaseAddress), sizeof(T), reinterpret_cast<uint8_t*>(&buffer));
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
		writeData((void*)lpBaseAddress, sizeof(T), (unsigned char*)&data);
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
	void writeDataImpl(void* lpAddress, int lenth, uint8_t* buffer);
	void readDataImpl(void* lpAddress, int lenth, uint8_t* buffer);

	void* hProcess;
	void* hwnd;
	uint remote_addr_;
};

