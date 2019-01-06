#pragma once
#include <cstdint>
#include <windows.h>

namespace native
{
	class keyboard_input
	{

	public:
		keyboard_input() {};

		inline bool pressed(const int32_t key, const bool toggle)
		{
			const auto key_down = GetAsyncKeyState(key) & 0x8000;

			if (!toggle)
			{
				return key_down;
			}

			const auto pressed_last_call = this->m_pressed_keys[key];
			const auto result = key_down && !pressed_last_call;
			this->m_pressed_keys[key] = key_down;

			return result;
		}

	private:
		bool m_pressed_keys[256];

	};
}
