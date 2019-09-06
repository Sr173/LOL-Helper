#include "game_call.h"
#include "AssemblyTools.hpp"
#include "base_address.h"
#include "game_manager.h"

game_call::game_call(uint32_t obj,std::shared_ptr<MemRWer> gmrw)
	:obj_(obj),game_memrwer_(gmrw)
{

}

void game_call::issue_order(int type,uint32_t position,uint32_t target_obj)
{
	AssemblyTools at;
	at.clear();
	at.pushad();
	at.push(0);
	at.push(0);
	at.push(0);
	at.push(target_obj);
	at.push(position);
	at.push(type);
	at.mov_ecx(game_memrwer_->read<uint>(base_address::my_hero));
	at.mov_eax(base_address::issue_order);
	at.call_eax();
	at.popad();
	at.ret();
	game_memrwer_->runRemoteThread(at.getCode());
}

bool game_call::is_hero()
{
	auto v2 = game_memrwer_->read<__int8>(obj_ + 0x49);
	auto v3 = obj_ + 0x48;
	auto v5 = game_memrwer_->read<__int8>(v3 + 0x14);
	auto v11 = game_memrwer_->read<uint>(v3 + v2 * 4 + 4);
	for (uint i = 0, v6 = game_memrwer_->read<uint>(v3 + 0x10); i < v5; i++, v6++)
	{
		auto v7 = game_memrwer_->read<uint>(v6);
		v11 ^= ~(v6 ^ 0xABB08BA0);
	}
	auto v8 = game_memrwer_->read<__int8>(v3 + 0x15);
	if (v8)
	{
		for (int i = 4 - v8; i < 4; ++i)
			* (reinterpret_cast<__int8*>(&v11) + i) ^= game_memrwer_->read<uint>(v3 + i + 16) ^ 0xA0;
	}
	return (v11 & 0x1000) != 0;
}

uint32_t game_call::get_position_point() const
{
	return obj_ + 0x1A0;
}

float game_call::hp() const
{
	return game_memrwer_->read<float>(obj_ + 0xDD4);
}

int game_call::camp() const
{
	return game_memrwer_->read<int>(obj_ + 0x44);
}

int game_call::attack_area() const
{
	return game_memrwer_->read<int>(obj_ + 0x1888);
}

uint32_t game_call::get_mouse_world_position_point()
{
	return GAME_MANAGER_INSTANCE->game_memrwer->read<uint32_t>(base_address::mouse, { 0x14 }) + 0x10;
}

uint32_t game_call::get_last_choice_obj()
{
	return GAME_MANAGER_INSTANCE->game_memrwer->read<uint32_t>(base_address::panel, { 0x78,0xC,0xB4 }) + 0x10;
}

float game_call::get_game_time()
{
	return GAME_MANAGER_INSTANCE->game_memrwer->read<float>(base_address::game_time);
}

float game_call::get_my_hero_attack_speed()
{
	return GAME_MANAGER_INSTANCE->game_memrwer->read<uint32_t>(base_address::panel, { 0x80,0x4,0x170,0x18 });
}

float game_call::calc_obj_instance(uint32_t obj1, uint32_t obj2)
{
	struct
	{
		float x;
		float y;
		float z;
	} pos[2];
	uint32_t p1 = game_call(obj1, GAME_MANAGER_INSTANCE->game_memrwer).get_position_point();
	uint32_t p2 = game_call(obj2, GAME_MANAGER_INSTANCE->game_memrwer).get_position_point();
	GAME_MANAGER_INSTANCE->game_memrwer->readData(p1, sizeof(pos[0]), pos);
	GAME_MANAGER_INSTANCE->game_memrwer->readData(p2, sizeof(pos[0]), pos + 1);
	return sqrtf((pos[0].x - pos[1].x) * (pos[0].x - pos[1].x) + (pos[0].y - pos[1].y) * (pos[0].y - pos[1].y) + (pos[0].z - pos[1].z) * (pos[0].z - pos[1].z));
}

