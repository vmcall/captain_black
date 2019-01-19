#pragma once
#include <cstdint>
#include <d3d11.h>

class overlay_helper
{
	IDXGISwapChain* swapchain() const;
	std::uintptr_t get_vmt_function(std::int32_t function_index) const;
};