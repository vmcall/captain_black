#pragma once
#include <cstdint>
#include <d3d11.h>

namespace steam
{
	class overlay_helper
	{
	public:
		IDXGISwapChain* swapchain() const;
		std::uintptr_t get_function(std::int32_t function_index) const;
	};
}
