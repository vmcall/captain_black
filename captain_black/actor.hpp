#pragma once

#include "misc.hpp"
#include <cstdint>

namespace engine
{
	struct actor
	{
		static constexpr auto local_offset = 0x341FE08;

		char pad_0000[104];					// 000
		wchar_t* name;						// 068
		char pad_0070[160];					// 070
		vector3d position;					// 110
		char pad_011C[2780];				// 11C
		std::int32_t movement_speed;		// BF8
		std::int32_t attack_speed;			// BFC
		std::int32_t cast_speed;			// C00
		char pad_0C04[2];					// C04
		std::uint16_t movement_backwards;	// C06
	};
}