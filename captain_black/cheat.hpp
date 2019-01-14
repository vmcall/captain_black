#pragma once
#include <cstddef>
#include "actor.hpp"
#include "keyboard_input.hpp"
#include "lua.hpp"
#include "command_helper.hpp"

namespace bdo
{
	class cheat
	{
	public:
		cheat() noexcept;

		void start();
		void stop();

		void setup_hooks();
		void release_hooks();

		void handle_loop();

		void handle_code_patches();
		void handle_object_patches();
		void handle_local_patches();

		std::byte*			base();
		bdo::engine::actor* local_player();

		
		bdo::engine::lua&		lua();
		bdo::engine::command_helper& command_helper();
		native::keyboard_input& keyboard();


	private:
		std::byte*					m_base_address;
		native::keyboard_input		m_keyboard;
		bdo::engine::lua			m_lua;
		bdo::engine::command_helper m_command_helper;

	};
}
