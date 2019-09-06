#pragma once
#include "singleton.hpp"
#include "MemRWer.h"

class game_manager
{
	friend class Singleton<game_manager>;
public:
	using singleton = Singleton<game_manager>;
	void init();

	void game_loader_thread();
	std::shared_ptr<MemRWer> game_memrwer;
private:
	game_manager();
	~game_manager() = delete;
	void* current_game_hwnd_;
};

#define GAME_MANAGER_INSTANCE game_manager::singleton::get_singleton_ptr()