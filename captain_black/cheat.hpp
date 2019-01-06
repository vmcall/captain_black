#pragma once
#include <cstddef>
#include "engine.hpp"
#include "keyboard_input.hpp"

namespace bdo
{
	class cheat
	{
	public:
		cheat();
		void start();
		void stop();

		std::byte* get_base() const;
		bdo::engine::actor* get_local_player() const;
	private:
		std::byte* m_base_address;
		native::keyboard_input keyboard;

	};
}
