#pragma once
#include <cstddef>
#include "lua.hpp"
#include "actor.hpp"
#include "cache.hpp"
#include "steam_overlay.hpp"
#include "keyboard_input.hpp"
#include "command_helper.hpp"

class captain
{
public:
	captain() noexcept;

	void start();
	void stop();

	void setup_hooks();
	void release_hooks();

	void handle_loop();

	void handle_code_patches();
	void cache_field_effects();
	void handle_local_patches();

	cache_t&				cache();
	std::byte*				base();
	engine::actor*			local_player();
	engine::lua&			lua();
	engine::command_helper&	command_helper();
	native::keyboard_input&	keyboard();
	steam::overlay_helper&	overlay_helper();


private:
	steam::overlay_helper	m_overlay_helper;
	std::byte*				m_base_address;
	native::keyboard_input	m_keyboard;
	engine::lua				m_lua;
	engine::command_helper	m_command_helper;
	cache_t					m_cache;

};
