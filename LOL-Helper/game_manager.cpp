#include "game_manager.h"
#include "windows.h"

void game_manager::init()
{
	std::thread(&game_manager::game_loader_thread, this).detach();
}

void game_manager::game_loader_thread()
{
	while (true)
	{
		auto game_hwnd = FindWindow(L"RiotWindowClass", L"League of Legends (TM) Client");
		if (game_hwnd != current_game_hwnd_)
		{
			current_game_hwnd_ = game_hwnd;
			game_memrwer->open_by_window(current_game_hwnd_);
		}

		Sleep(1000);
	}
}

game_manager::game_manager()
	:game_memrwer(std::make_shared<MemRWer>()),
	 current_game_hwnd_(nullptr)
{

}
