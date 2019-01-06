#define NOMINMAX

#include "cheat.hpp"
#include "logger.hpp"

#include <windows.h>
#include <cstdint>
#include <chrono>
#include <thread>
#include <numeric>


bdo::cheat::cheat()
{
	this->m_base_address = reinterpret_cast<std::byte*>( GetModuleHandleW(nullptr) );
}

void bdo::cheat::start()
{
	// SPAWN CONSOLE
	logger::initialise();
	logger::log("Welcome to CaptainBlack v1");
	logger::log("Patching anti-cheat...");
	logger::log_pointer("Base", this->get_base());
	logger::log_pointer("SelfPlayerActorProxy", this->get_local_player());

	// ANTI-CHEAT SPEED-CAP FUNCTION
	auto function = this->get_base() + bdo::engine::speedcap_function_offset;

	// WRITE RET INSTRUCTION TO PATCH IT
	constexpr auto ret_opcode = 0xC3;
	*function = static_cast<std::byte>(ret_opcode);

	logger::log_pointer("XignCode::SpeedHackCap", function);

	logger::log("Patched..!");


	logger::log("Dumping ObjectSceneInfo...");

	auto object_scene_info = bdo::engine::container_wrapper(reinterpret_cast<bdo::engine::pa_container*>(0x14320B170));

	for (auto entry : object_scene_info.variables())
	{
		printf("[%04llX] %s\n", entry.second.offset, entry.first.c_str());
	}
	logger::log("Dumped..!");


	logger::log("Fetching ObjectSceneInfo->cam_maxDistanceFromCharacter");

	auto max_distance = object_scene_info.get<float>("cam_maxDistanceFromCharacter");

	logger::log_pointer("cam_maxDistanceFromCharacter", max_distance);
	logger::log("Overwriting ObjectSceneInfo->cam_maxDistanceFromCharacter");

	*max_distance = 999999.f;


	logger::log("Overwriting SelfPlayerActorProxy->movement_speed");
	logger::log("Overwriting SelfPlayerActorProxy->attack_speed");
	logger::log("Overwriting SelfPlayerActorProxy->cast_speed");


	while (!this->keyboard.pressed(VK_END, false))
	{
		// WRITE SPEED VALUES
		if (this->get_local_player())
		{
			constexpr auto max_movement = std::numeric_limits<std::int32_t>::max();
			constexpr auto max_attack = 5999999;
			this->get_local_player()->attack_speed = max_attack;
			this->get_local_player()->cast_speed = max_attack;
			this->get_local_player()->movement_speed = max_movement;
		}

		// SLEEP
		std::this_thread::sleep_for(std::chrono::milliseconds(1));
	}

	this->stop();
}

void bdo::cheat::stop()
{
	// RELEASE CONSOLE
	logger::release();
}

std::byte* bdo::cheat::get_base() const
{
	return this->m_base_address;
}

bdo::engine::actor* bdo::cheat::get_local_player() const
{
	return *reinterpret_cast<bdo::engine::actor**>(this->get_base() + bdo::engine::local_actor_offset);
}
