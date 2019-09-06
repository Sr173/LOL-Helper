#include "game_manager.h"
#include "windows.h"
#include "MainWindow.h"
#include "AssemblyTools.hpp"
#include <iostream>

void game_manager::init()
{
	std::thread(&game_manager::game_loader_thread, this).detach();
}

void game_manager::game_loader_thread()
{
	while (true)
	{
		const auto game_hwnd = FindWindow(L"RiotWindowClass", L"League of Legends (TM) Client");
		if (game_hwnd != nullptr && game_hwnd != current_game_hwnd_)
		{
			current_game_hwnd_ = game_hwnd;
			game_memrwer->open_by_window(current_game_hwnd_);
			auto black_memory = get_black_memory(0x120);
			game_memrwer->set_execute_code_memory(black_memory);
			MAIN_WINDOW_INSTANCE->game_status_text = u8"等待注入";

			DWORD game_pid;

			GetWindowThreadProcessId(game_hwnd, &game_pid);

			int oldId = GetCurrentProcessId();
			_asm {
				mov eax, dword ptr fs : [0x18] ;
				lea eax, [eax + 0x20];
				mov ebx, game_pid;
				mov dword ptr ds : [eax] , ebx;
			}
			uint oldProc = GetWindowLongW(static_cast<HWND>(current_game_hwnd_), GWL_WNDPROC);
			_asm {
				mov eax, dword ptr fs : [0x18] ;
				lea eax, [eax + 0x20];
				mov ebx, oldId;
				mov dword ptr ds : [eax] , ebx;
			}

			auto get_call_addr = game_memrwer->read<uint>(oldProc + 0x38 + 1);
			get_call_addr = oldProc + 0x38 + get_call_addr + 5 + 4;
			auto call_addr = game_memrwer->read<uint>(get_call_addr);
			std::cout << std::hex << call_addr << std::endl;

			while (call_addr == 0 && IsWindow(game_hwnd))
			{
				call_addr = game_memrwer->read<uint>(get_call_addr);
				Sleep(100);
			}

			if (game_memrwer->read<byte>(call_addr, { 0 }) == 0x55) {

				DWORD old_protected;
				VirtualProtectEx(game_memrwer->get_game_handle(), reinterpret_cast<LPVOID>(black_memory), 0x120, PAGE_EXECUTE_READWRITE, &old_protected);
				game_memrwer->read<uint>(game_memrwer->read<uint>(oldProc + 0x39));
				AssemblyTools at;
				at.write<byte>(0x83); at.write<byte>(0x7C); at.write<byte>(0x24); at.write<byte>(0x08); at.write<byte>(0x34);//cmp [esp + C],0x34
				at.write<byte>(0x75); at.write<byte>(0x06);//jne 05
				at.write<byte>(0xE8); at.write<byte>(0xF4); at.write<byte>(0xFE); at.write<byte>(0xFF); at.write<byte>(0xFF);
				at.write<byte>(0xC3);
				at.push(game_memrwer->read<uint>(call_addr));
				at.ret();
				VirtualProtectEx(game_memrwer->get_game_handle(), reinterpret_cast<LPVOID>(black_memory), 0x120, PAGE_EXECUTE_READWRITE, &old_protected);
				game_memrwer->writeData(reinterpret_cast<LPVOID>(black_memory + 0x100), at.getCode().size(), at.getCode().data());
				game_memrwer->write<uint>(call_addr, black_memory + 0x100);
			}
			MAIN_WINDOW_INSTANCE->game_status_text = u8"注入完毕";
		}

		if (game_hwnd == nullptr)
		{
			MAIN_WINDOW_INSTANCE->game_status_text = u8"未找到游戏";
		}
		Sleep(1000);
	}
}

int game_manager::get_black_memory(int size)
{
	const auto nt_dll_base = GetModuleHandle(L"ntdll.dll");
	MEMORY_BASIC_INFORMATION memInfo;
	for (VirtualQuery(nt_dll_base, &memInfo, sizeof(memInfo)); VirtualQuery(LPVOID(reinterpret_cast<int>(memInfo.BaseAddress) + memInfo.RegionSize), &memInfo, sizeof(memInfo));) {
		if (memInfo.Protect == PAGE_READWRITE) {
			for (int i = reinterpret_cast<int>(memInfo.BaseAddress), length = 0; i < reinterpret_cast<int>(memInfo.BaseAddress) + memInfo.RegionSize; i++) {
				if (*reinterpret_cast<BYTE*>(i) == 0) {
					length++;
				}
				else {
					length = 0;
				}

				if (length >= size) {
					return i;
				}
			}
		}
	}
	return 0;
}
game_manager::game_manager()
	:game_memrwer(std::make_shared<MemRWer>()),
	 current_game_hwnd_(nullptr)
{

}
