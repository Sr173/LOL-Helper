#include "MainWindow.h"
#include "imgui.h"

void main_window::render()
{
	ImGui::Begin(u8"��Ϸ����");
	ImGui::Text(u8"��Ϸ״̬��%s",game_status_text.data());
	ImGui::End();
}