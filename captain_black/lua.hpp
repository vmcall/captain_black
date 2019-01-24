#pragma once

#include <cstdint>

namespace engine
{
	class lua
	{
	public:

		// OFFSETS
		static constexpr auto do_string_offset = 0xF445D0;
		static constexpr auto gettop_offset = 0x1778360;

		explicit lua(std::uint64_t base_address) noexcept : m_base_address(base_address)
		{
			// CACHE FUNCTION POINTERS
			this->do_string = reinterpret_cast<engine::lua::do_string_t>(base_address + do_string_offset);
			this->gettop = reinterpret_cast<engine::lua::gettop_t>(base_address + gettop_offset);
		}

		using do_string_t = void*(__fastcall*)(std::int64_t state, const char* command, std::int64_t length);
		do_string_t do_string;

		using gettop_t = void*(__fastcall*)(std::int64_t state);
		gettop_t gettop;

	private:
		std::uint64_t m_base_address;

	};
}