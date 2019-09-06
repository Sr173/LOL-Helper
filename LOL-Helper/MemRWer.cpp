#include "MemRWer.h"
#include "windows.h"

MemRWer::MemRWer()
{

}

MemRWer::~MemRWer()
{

}

bool MemRWer::open_by_window(void* hwnd)
{
	this->hwnd = hwnd;
	DWORD id;
	GetWindowThreadProcessId(static_cast<HWND>(hwnd), &id);
	hProcess = OpenProcess(PROCESS_ALL_ACCESS, false, id);
	return hProcess != nullptr;
}

void MemRWer::readDataImpl(LPVOID lpAddress, int lenth, byte * buffer)
{
	ReadProcessMemory(hProcess, lpAddress, buffer, lenth, 0);
}

void MemRWer::writeDataImpl(LPVOID lpAddress, int lenth, byte* buffer) {
	WriteProcessMemory(hProcess, lpAddress, buffer, lenth, 0);
}

int MemRWer::runRemoteThread(std::vector<byte> opcode)
{
	writeData(remote_addr_, opcode.size(), opcode.data());
	int result = SendMessageA((HWND)hwnd, 0x34, 0, 0);
	std::fill(opcode.begin(), opcode.end(), 0);
	writeData(remote_addr_, opcode.size(), opcode.data());
	return result;
}


std::wstring MemRWer::readWstring(uint addr) {
	WCHAR buffer[200];
	readData((LPVOID)addr, sizeof(WCHAR) * 200, (byte*)buffer);
	return buffer;
}

std::string MemRWer::readString(uint addr)
{
	CHAR buffer[200];
	readData((LPVOID)addr, sizeof(CHAR) * 200, (byte*)buffer);
	return buffer;
}

void MemRWer::writeWstring(uint addr, std::wstring str)
{
	writeData((LPVOID)addr, str.size() * 2 + 2, (byte*)str.c_str());
}

int MemRWer::decryption(uint addr) {
	return 0;
}