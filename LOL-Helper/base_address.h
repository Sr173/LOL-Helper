#pragma once
#include <cstdint>

namespace base_address {
	const uint32_t panel = 0x273CD00;//6A 01 85 C9 74 0A
	const uint32_t is_cheat = 0x1AED9C0;//83 3D ?? ?? ?? ?? 01 57
	const uint32_t game = 0x32CE25C;//6A 44 FF 35 ?? ?? ?? ?? E8 ?? ?? ?? ?? 83 C4 08 C7 05 ?? ?? ?? ?? 00 00 00 00 C3
	const uint32_t mouse = 0x1AF0270;//74 15 83 E8 01 75 10
	//0087DE67    6A 01           push 0x1
	//0087DE69    6A 00           push 0x0
	//0087DE6B    6A 00           push 0x0
	//0087DE6D    6A 00           push 0x0
	//0087DE6F    8D81 E0010000   lea eax, dword ptr ds : [ecx + 0x1E0]
	//0087DE75    50              push eax
	//0087DE76    6A 0A           push 0xA
	//0087DE78    E8 3337D4FF     call 005C15B0
	const uint32_t issue_order = 0x005C15B0;//6A 01 6A 00 6A 00 6A 00 8D 81 E0 01 00 00 50 6A 0A
	const uint32_t my_hero = 0x338F7D4;//0F 84 63 02 00 00 53 55 56
	const uint32_t table = 0x1AF057C;//8B C8 2B CE C1 F9 02 83 F9 1E
	const uint32_t game_time = 0x338AA88;//8B C8 2B CE C1 F9 02 83 F9 1E
}
