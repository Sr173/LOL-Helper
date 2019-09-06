#include "MainWindow.h"
#include "imgui.h"

void main_window::render()
{
	ImGui::Begin(u8"游戏功能");
	ImGui::Text(u8"游戏状态：%s",game_status_text.data());
	ImGui::End();
}