#define NOMINMAX

#include "cheat.hpp"
#include "logger.hpp"
#include "global.hpp"
#include "anticheat.hpp"
#include "variable_container.hpp"
#include "Minhook/include/MinHook.h"

#include <windows.h>
#include <cstdint>
#include <chrono>
#include <thread>
#include <numeric>


bdo::cheat::cheat() noexcept :
	m_base_address(reinterpret_cast<std::byte*>(GetModuleHandleW(nullptr))), 
	m_lua(reinterpret_cast<std::uint64_t>(this->base()))
{

}

void bdo::cheat::start()
{
	// SPAWN CONSOLE
	logger::initialise();
	logger::log("Welcome to CaptainBlack v1");
	logger::log_pointer("Base", this->base());
	logger::log_pointer("SelfPlayerActorProxy", this->local_player());

	// SETUP HOOKS
	this->setup_hooks();

	// DO CODE PATCHES
	this->handle_code_patches();

	// DO OBJECT PATCHES
	this->handle_object_patches();

	//logger::log("Overwriting SelfPlayerActorProxy->movement_speed");
	//logger::log("Overwriting SelfPlayerActorProxy->attack_speed");
	//logger::log("Overwriting SelfPlayerActorProxy->cast_speed");


	// CHEAT LOOP
	while (!this->keyboard().pressed(VK_END, false))
	{
		// HANDLE ITERATIVE PATCHES
		this->handle_loop();

		// SLEEP
		std::this_thread::sleep_for(std::chrono::milliseconds(1));
	}

	// RELEASE CHEAT
	this->stop();
}

void bdo::cheat::stop()
{
	// RELEASE CONSOLE
	logger::release();

	// RELEASE HOOKS
	this->release_hooks();
}

void bdo::cheat::setup_hooks()
{
	logger::log("Hooking...");

	// SETUP MINHOOK
	MH_Initialize();

	// HOOK LUA::DOSTRING
	MH_CreateHook(this->lua().do_string, bdo::global::do_string_hook, reinterpret_cast<void**>(&bdo::global::do_string_original));
	MH_EnableHook(this->lua().do_string);

	logger::log("Hooked..!");
}

void bdo::cheat::release_hooks()
{
	// UNHOOK LUA::DOSTRING
	MH_DisableHook(this->lua().do_string);
}

void bdo::cheat::handle_loop()
{
	// HANDLE LOCAL-ACTOR PATCHES
	this->handle_local_patches();
}

void bdo::cheat::handle_code_patches()
{
	logger::log("Patching anti-cheat...");

	// XignCode::SPEEDCAP FORCES NORMAL-RANGE VALUES FOR SPEED MODIFIERS
	auto speedcap_function = this->base() + bdo::anticheat::speedcap_function_offset;
	logger::log_pointer("XignCode::SpeedHackCap", speedcap_function);

	// WRITE RET INSTRUCTION TO PATCH IT :')
	constexpr auto ret_opcode = 0xC3;
	*speedcap_function = static_cast<std::byte>(ret_opcode);

	logger::log("Patched..!");
}

void bdo::cheat::handle_object_patches()
{
	logger::log("Dumping ObjectSceneInfo...");

	auto object_scene_info = bdo::engine::container_wrapper(reinterpret_cast<bdo::engine::pa_container*>(0x14320B170));

	//for (auto entry : object_scene_info.variables())
	//{
	//	printf("[%04llX] %s\n", entry.second.offset, entry.first.c_str());
	//}

	logger::log("Dumped..!");

	logger::log("Patching ObjectSceneInfo->cam_maxDistanceFromCharacter");
	*object_scene_info.get<float>("cam_maxDistanceFromCharacter") = 999999.f;
}

void bdo::cheat::handle_local_patches()
{
	if (this->local_player())
	{
		// ATTACK SPEED, LIMITED TO NOT TIME OUT FOR ATTACKING TOO FAST :')
		constexpr auto max_attack = 5999999;
		this->local_player()->attack_speed = max_attack;
		this->local_player()->cast_speed = max_attack;

		// MOVEMENT SPEED, JUST MAX :')
		constexpr auto max_movement = std::numeric_limits<std::int32_t>::max();
		this->local_player()->movement_speed = max_movement;
	}
}

std::byte* bdo::cheat::base()
{
	return this->m_base_address;
}

bdo::engine::actor* bdo::cheat::local_player()
{
	return *reinterpret_cast<bdo::engine::actor**>(this->base() + bdo::engine::actor::local_offset);
}

bdo::engine::lua& bdo::cheat::lua()
{
	return this->m_lua;
}

native::keyboard_input& bdo::cheat::keyboard()
{
	return this->m_keyboard;
}
