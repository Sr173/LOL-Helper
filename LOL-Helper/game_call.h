#pragma once
#include "vector"
#include "MemRWer.h"

class game_call
{
public:
	enum issue_order_type
	{
		move = 2,
		attack = 3
	};
	game_call(uint32_t obj, std::shared_ptr<MemRWer> game_memrwer);
	void issue_order(int type, uint32_t position, uint32_t target_obj);
	bool is_hero();
	uint32_t get_position_point() const;
	float hp() const;
	int camp() const;
	int attack_area() const;
	std::string name() const;
	const auto pointer() { return obj_; }
	static uint32_t get_mouse_world_position_point();
	static uint32_t get_last_choice_obj();
	static float get_game_time();
	static float get_my_hero_attack_speed();
	static float calc_obj_instance(uint32_t obj1, uint32_t obj2);
private:
	uint32_t obj_;
	std::shared_ptr<MemRWer> game_memrwer_;
};