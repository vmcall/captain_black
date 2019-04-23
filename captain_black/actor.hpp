#pragma once

#include "misc.hpp"
#include <cstdint>

namespace engine
{
	struct actor
	{
		static constexpr auto local_offset = 0x34736D8;

		class N00004159* N00000015; //0x0000
		char pad_0008[24]; //0x0008
		bool N00000019; //0x0020
		char pad_0021[59]; //0x0021
		uint32_t type; //0x005C
		char pad_0060[8]; //0x0060
		wchar_t* name; //0x0068
		class N0000033D* N00000023; //0x0070
		char pad_0078[24]; //0x0078
		uint16_t hit_count; //0x0090
		char pad_0092[158]; //0x0092
		float N00000037; //0x0130
		float N00003B21; //0x0134
		float N00003B23; //0x0138
		char pad_013C[132]; //0x013C
		class N00003A16* N00000049; //0x01C0
		char pad_01C8[464]; //0x01C8
		class N00004145* N00000085; //0x0398
		char pad_03A0[100]; //0x03A0
		int32_t combat_stance; //0x0404
		char pad_0408[1992]; //0x0408
		float N00000196; //0x0BD0
		float N00004153; //0x0BD4
		char pad_0BD8[64]; //0x0BD8
		int32_t movement_speed; //0x0C18
		int32_t attack_speed; //0x0C1C
		int32_t casting_speed; //0x0C20
		int16_t N00000E8E; //0x0C24
		int16_t N00003EC3; //0x0C26
		int16_t N00003EBF; //0x0C28
		int16_t N00003EC5; //0x0C2A
		char pad_0C2C[112]; //0x0C2C
		int8_t N000001B1; //0x0C9C
		char pad_0C9D[703]; //0x0C9D
		int32_t N00000209; //0x0F5C
		int32_t adrenalin; //0x0F60
		char pad_0F64[28]; //0x0F64
		uint32_t N00004150; //0x0F80
		char pad_0F84[224]; //0x0F84
		float critical_hit_rate; //0x1064
		char pad_1068[12]; //0x1068
		uint64_t N000003CE; //0x1074
		char pad_107C[16]; //0x107C
		float regen; //0x108C
		char pad_1090[140]; //0x1090
		int32_t level; //0x111C
		char pad_1120[876]; //0x1120
		bool pvp_available; //0x148C
		bool pvp_activated; //0x148D
		char pad_148E[9358]; //0x148E
		float N000008E3; //0x391C
		float N00003B29; //0x3920
		float N000008E4; //0x3924
		float N00003B2C; //0x3928
		float N000008E5; //0x392C
		float N00003B2F; //0x3930
		char pad_3934[3980]; //0x3934
		bool N0000302D; //0x48C0
		char pad_48C1[2055]; //0x48C1
		bool adrenalin_enabled; //0x50C8
		char pad_50C9[5235]; //0x50C9

	};
}