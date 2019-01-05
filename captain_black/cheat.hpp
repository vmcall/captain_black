#pragma once
#include <cstddef>
#include "engine.hpp"

namespace captain_black
{
	class cheat
	{
	public:
		cheat();
		void start();
		void stop();

		std::byte* get_base() const;
		captain_black::engine::actor* get_local_player() const;
	private:
		std::byte* m_base_address;
		captain_black::engine::actor* m_local_actor;
	};
}
