#pragma once
#include "singleton.hpp"
#include <string>

class main_window
{
	friend class Singleton<main_window>;
public:
	using singleton = Singleton<main_window>;
	void render();

	std::string game_status_text;
private:
	main_window() = default;
	~main_window() = delete;
};

#define MAIN_WINDOW_INSTANCE main_window::singleton::get_singleton_ptr()