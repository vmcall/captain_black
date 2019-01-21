#define NOMINMAX

#include "captain.hpp"
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


captain::captain() noexcept :
	m_base_address(reinterpret_cast<std::byte*>(GetModuleHandleW(nullptr))), 
	m_lua(reinterpret_cast<std::uint64_t>(this->base()))
{

}

void captain::start()
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
}

void captain::stop()
{
	// RELEASE CONSOLE
	logger::release();

	// RELEASE HOOKS
	this->release_hooks();
}

void captain::setup_hooks()
{
	logger::log("Hooking...");

	// SETUP MINHOOK
	MH_Initialize();

	// HOOK LUA::DOSTRING
	MH_CreateHook(this->lua().gettop, global::gettop_hook, reinterpret_cast<void**>(&global::gettop_original));
	MH_EnableHook(this->lua().gettop);

	// HOOK	PRESENT
	auto present = this->overlay_helper().get_present();

	logger::log_pointer("present", reinterpret_cast<void*>(present));
	//logger::log_pointer("pSwapChain", pSwapChain);
	MH_CreateHook(reinterpret_cast<void*>(present), global::present_hook, reinterpret_cast<void**>(&global::present_original));
	MH_EnableHook(reinterpret_cast<void*>(present));

	logger::log("Hooked..!");
}

void captain::release_hooks()
{
	// UNHOOK LUA::DOSTRING
	MH_DisableHook(this->lua().gettop);

	// UNHOOK PRESENT
	auto present = this->overlay_helper().get_present();
	MH_DisableHook(reinterpret_cast<void*>(present));
}

void captain::handle_loop()
{
	// HANDLE LOCAL-ACTOR PATCHES
	this->handle_local_patches();
}

void captain::handle_code_patches()
{
	logger::log("Patching anti-cheat...");

	// XignCode::SPEEDCAP FORCES NORMAL-RANGE VALUES FOR SPEED MODIFIERS
	auto speedcap_function = this->base() + anticheat::speedcap_function_offset;
	logger::log_pointer("game::cap_speed", speedcap_function);

	// WRITE RET INSTRUCTION TO PATCH IT :')
	constexpr auto ret_opcode = 0xC3;
	*speedcap_function = static_cast<std::byte>(ret_opcode);

	logger::log("Patched..!");
}

void captain::handle_field_effects()
{
	logger::log("Dumping FieldEffectInfo...");

	constexpr auto field_effect_info_offset = 0x3244370;
	auto raw_container = reinterpret_cast<engine::pa_container*>(this->base() + field_effect_info_offset);
	auto field_effect_info = engine::container_wrapper(raw_container);

	//for (auto entry : field_effect_info.variables())
	//{
	//	printf("[%04llX] %s\n", entry.second.offset, entry.first.c_str());
	//}

	logger::log("Dumped..!");
	logger::log("Patching...");

	// CAMERA DISTANCE
	if (global::options.camera_distance)
		*field_effect_info.get<float>("cam_maxDistanceFromCharacter") = 9999.f;
	
	// STEP HEIGHT
	if (global::options.step)
		*field_effect_info.get<float>("ch_stepOffset") = 9999.f;

	logger::log("Patched..!");

	// CLIFF LEAN
	//*object_scene_info.get<float>("_cc_nearCliffCheckDistance") = 0.f;
	
}

void captain::handle_local_patches()
{
	if (this->local_player())
	{
		// ATTACK SPEED, LIMITED TO NOT TIME OUT FOR ATTACKING TOO FAST :')
		constexpr auto max_attack = std::numeric_limits<std::int32_t>::max();// 9999999;

		// ATTACK SPEED
		if (global::options.attack_speed)
			this->local_player()->attack_speed = max_attack;

		// CAST SPEED
		if (global::options.cast_speed)
			this->local_player()->cast_speed = max_attack;

		// MOVEMENT SPEED
		constexpr auto max_movement = std::numeric_limits<std::int32_t>::max();
		if (global::options.movement_speed)
			this->local_player()->movement_speed = max_movement;
	}
}

std::byte* captain::base()
{
	return this->m_base_address;
}

engine::actor* captain::local_player()
{
	return *reinterpret_cast<engine::actor**>(this->base() + engine::actor::local_offset);
}

engine::lua& captain::lua()
{
	return this->m_lua;
}

engine::command_helper& captain::command_helper()
{
	return this->m_command_helper;
}

native::keyboard_input& captain::keyboard()
{
	return this->m_keyboard;
}

steam::overlay_helper& captain::overlay_helper()
{
	return this->m_overlay_helper;
}
