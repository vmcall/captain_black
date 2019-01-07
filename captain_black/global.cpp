#include "global.hpp"

#include <iostream>

bdo::cheat bdo::global::cheat = {};
bdo::engine::lua::do_string_t bdo::global::do_string_original = nullptr;

void* __fastcall bdo::global::do_string_hook(std::int64_t state, const char* command, std::int64_t length)
{
	if (state && command)
	{
		printf("[lua::do_string] %s - (%p)\n", command, reinterpret_cast<void*>(state));
	}

	return bdo::global::do_string_original(state, command, length);
}
