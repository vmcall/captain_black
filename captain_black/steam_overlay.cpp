#include "steam_overlay.hpp"

IDXGISwapChain* overlay_helper::swapchain() const
{
	auto module_handle = reinterpret_cast<std::uintptr_t>(GetModuleHandleW(L"GameOverlayRenderer64.dll"));
	auto tmp = *reinterpret_cast<std::uintptr_t*>(module_handle + 0x15BA00);
	return *reinterpret_cast<IDXGISwapChain**>(tmp + 0xD8);
}

std::uintptr_t overlay_helper::get_vmt_function(const std::int32_t function_index) const
{
	auto virtual_method_table = *reinterpret_cast<std::uintptr_t**>(this->swapchain());
	return virtual_method_table[function_index];
}
