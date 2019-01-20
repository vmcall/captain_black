#pragma once
#include <cstdint>
#include <d3d11.h>

namespace steam
{
	class overlay_helper
	{
	public:
		std::uintptr_t get_present() const;
	};
}
