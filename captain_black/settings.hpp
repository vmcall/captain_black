#pragma once
#define NOMINMAX
#include <numeric>

struct settings
{
	bool show_menu = true;
	bool camera_distance = true;
	bool movement_speed = true;
	bool attack_speed = true;
	bool cast_speed = true;
	bool step = true;
	bool speed_low = true;
	bool chams;
	bool occlusion;
	std::int32_t selected_stride;
};