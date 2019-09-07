#include "MainWindow.h"
#include "imgui.h"
#include "game_manager.h"
#include "game_call.h"
#include <ios>
#include <ostream>
#include "base_address.h"

void main_window::render()
{
	static auto selected = 0;
	ImGui::Begin(u8"游戏功能");
	ImGui::Text(u8"游戏状态：%s",game_status_text.data());
	ImGui::Columns(9, u8"游戏数据"); // 4-ways, with border
	ImGui::Separator();
	ImGui::Text(u8"英雄ID"); ImGui::NextColumn();
	ImGui::Text(u8"Q技能"); ImGui::NextColumn();
	ImGui::Text(u8"W技能"); ImGui::NextColumn();
	ImGui::Text(u8"E技能"); ImGui::NextColumn();
	ImGui::Text(u8"R技能"); ImGui::NextColumn();
	ImGui::Text(u8"D技能"); ImGui::NextColumn();
	ImGui::Text(u8"F技能"); ImGui::NextColumn();
	ImGui::Text(u8"血量"); ImGui::NextColumn();
	ImGui::Text(u8"距离"); ImGui::NextColumn();
	int index = 0;

	const auto end = GAME_MANAGER_INSTANCE->game_memrwer->read<uint>(0x1AF0580);
	for (auto i = GAME_MANAGER_INSTANCE->game_memrwer->read<uint>(0x1AF057C); i < end; i += 4) {
		game_call obj(GAME_MANAGER_INSTANCE->game_memrwer->read<uint>(i), GAME_MANAGER_INSTANCE->game_memrwer);

		if (obj.is_hero()) {
			ImColor color(0xFF, 0xFF, 0xFF);
			if (obj.camp() != 100)
			{
				color = ImColor(0xFF, 0, 0);
			}
			ImGui::TextColored(color,obj.name().data()); ImGui::NextColumn();
			for (int i = 0; i < 6; i++) {
				if (GAME_MANAGER_INSTANCE->game_memrwer->read<float>(obj.pointer() + 0x1FCC, { 0x438,uint(0x508 + i * 4),0x28 }) 
					- GAME_MANAGER_INSTANCE->game_memrwer->read<float>(base_address::game_time) > 0) {
					ImGui::TextColored(color,std::to_string(GAME_MANAGER_INSTANCE->game_memrwer->read<float>(obj.pointer() + 0x1FCC, { 0x438,uint(0x508 + i * 4),0x28 })
						- GAME_MANAGER_INSTANCE->game_memrwer->read<float>(base_address::game_time)).data());
				}else
				{
					ImGui::TextColored( color, "0");
				}
				ImGui::NextColumn();
			}

			ImGui::TextColored(color, std::to_string(obj.hp()).data()); ImGui::NextColumn();
			ImGui::TextColored(color, std::to_string(game_call::calc_obj_instance(obj.pointer(),
				GAME_MANAGER_INSTANCE->game_memrwer->read<uint>(base_address::my_hero))).data());
			ImGui::NextColumn();
		}
	}

	game_call self_obj(GAME_MANAGER_INSTANCE->game_memrwer->read<uint>(base_address::my_hero), GAME_MANAGER_INSTANCE->game_memrwer);
	float posion[3];
	GAME_MANAGER_INSTANCE->game_memrwer->readData(self_obj.get_position_point(), sizeof(posion), posion);
	ImGui::Columns(1);
	ImGui::Separator();

	ImGui::Text(u8"当前英雄坐标 (%f,%f,%f)", posion[0], posion[1], posion[2]);
	
	float world_posion[3];
	GAME_MANAGER_INSTANCE->game_memrwer->readData(game_call::get_mouse_world_position_point(), sizeof(world_posion), world_posion);
	ImGui::Text(u8"鼠标指针坐标 (%f,%f,%f)", world_posion[0], world_posion[1], world_posion[2]);

	ImGui::End();
}
