#include "steam_overlay.hpp"
#include <cstddef>
#include "logger.hpp"

std::uintptr_t steam::overlay_helper::get_present() const
{
	// mov r11,rsp
	// mov qword ptr ds : [r11 + 8], rbx
	// jmp d3d9.7FFEC81EAF67					<---
	// jmp gameoverlayrenderer64.7FFEA6B7B800

	auto module_handle = reinterpret_cast<std::uintptr_t>(GetModuleHandleW(L"GameOverlayRenderer64.dll"));
	auto trampoline_address = *reinterpret_cast<std::uintptr_t*>(module_handle + 0x145318);

	logger::log_pointer("trampoline_address", reinterpret_cast<void*>(trampoline_address));

	//auto instruction_address = trampoline_address + 0x7;
	//auto offset = *reinterpret_cast<std::int32_t*>(instruction_address + 1);
	//auto next_instruction = instruction_address + 0x5;
	//auto present_address = reinterpret_cast<std::uintptr_t>(next_instruction) + offset;

	return trampoline_address;
}
