#pragma once

#include <cstdint>
#include <cstddef>

class vtable
{
public:
	vtable(std::uintptr_t class_base) noexcept;
	

	// VALIDITY CHECK
	operator bool() const noexcept;

private:
	std::uintptr_t m_class_base;
	std::size_t m_size;
};