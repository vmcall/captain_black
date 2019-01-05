#include "cheat.hpp"
#include <windows.h>
#include <cstdint>
#include <chrono>
#include <thread>
#include "logger.hpp"

captain_black::cheat::cheat()
{
	this->m_base_address = reinterpret_cast<std::byte*>( GetModuleHandleW(nullptr) );
}

void captain_black::cheat::start()
{
	// SPAWN CONSOLE
	logger::initialise();
	logger::log("Patching anti-cheat...");
	logger::log_pointer("Base", this->get_base());
	logger::log_pointer("SelfPlayerActorProxy", this->get_base());

	// ANTI-CHEAT SPEED-CAP FUNCTION
	auto function = this->get_base() + captain_black::engine::speedcap_function_offset;

	// WRITE RET INSTRUCTION TO PATCH IT
	constexpr auto ret_opcode = 0xC3;
	*function = static_cast<std::byte>(ret_opcode);
	
	logger::log("Patched..!");

	logger::log("Overwriting SelfPlayerActorProxy->movement_speed");
	logger::log("Overwriting SelfPlayerActorProxy->attack_speed");
	logger::log("Overwriting SelfPlayerActorProxy->cast_speed");

	while (true)
	{
		// WRITE SPEED VALUES
		if (this->get_local_player())
		{
			this->get_local_player()->attack_speed = 9999999;
			this->get_local_player()->cast_speed = 9999999;
			this->get_local_player()->movement_speed = 9999999;
		}



		// SLEEP
		std::this_thread::sleep_for(std::chrono::milliseconds(1));
	}
}

void captain_black::cheat::stop()
{
	// RELEASE CONSOLE
	logger::release();

}

std::byte* captain_black::cheat::get_base() const
{
	return this->m_base_address;
}

captain_black::engine::actor* captain_black::cheat::get_local_player() const
{
	return *reinterpret_cast<captain_black::engine::actor**>(this->get_base() + captain_black::engine::local_actor_offset);
}
