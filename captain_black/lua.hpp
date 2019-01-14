#pragma once

#include <cstdint>

namespace bdo::engine
{
	class lua
	{
	public:

		// OFFSETS
		static constexpr auto do_string_offset = 0xF00180;
		static constexpr auto gettop_offset = 0x1730700;

		explicit lua(std::uint64_t base_address) noexcept : m_base_address(base_address)
		{
			// CACHE FUNCTION POINTERS
			this->do_string = reinterpret_cast<bdo::engine::lua::do_string_t>(base_address + do_string_offset);
			this->gettop = reinterpret_cast<bdo::engine::lua::gettop_t>(base_address + gettop_offset);
		}

		using do_string_t = void*(__fastcall*)(std::int64_t state, const char* command, std::int64_t length);
		do_string_t do_string;

		using gettop_t = void*(__fastcall*)(std::int64_t state);
		gettop_t gettop;

	private:
		std::uint64_t m_base_address;

	};
}