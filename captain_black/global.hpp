#pragma once
#include "cheat.hpp"

namespace bdo::global
{
	extern bdo::cheat cheat;


	extern bdo::engine::lua::do_string_t do_string_original;
	extern void* __fastcall do_string_hook(std::int64_t state, const char* command, std::int64_t length);
}