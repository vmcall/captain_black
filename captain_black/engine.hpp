#pragma once
#include <cstdint>

namespace captain_black::engine
{
	constexpr auto speedcap_function_offset = 0x89E2D1;
	constexpr auto local_actor_offset = 0x33AE948;

	struct vector3d
	{
		float x;
		float y;
		float z;
	};

	struct actor
	{
		char pad_0000[104]; //0x0000
		wchar_t* name; //0x0068
		char pad_0070[160]; //0x0070
		vector3d position; //0x0110
		char pad_011C[2780]; //0x011C
		std::uint32_t movement_speed; //0x0BF8
		std::uint32_t attack_speed; //0x0BFC
		std::uint32_t cast_speed; //0x0C00
		char pad_0C04[2]; //0x0C04
		std::uint16_t movement_backwards; //0x0C06
	}; //Size: 0x4D08

}