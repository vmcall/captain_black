#pragma once
#include <cstdint>
#include <memory>
#include <windows.h>
#include <map>

#include "vtable.hpp"

class vtable_hook
{
public:
	explicit vtable_hook(std::uintptr_t class_base) : m_original_vtable(class_base), m_vtable(m_original_vtable)
	{

	}

	bool hook(std::int32_t index, void* hook);
	bool restore_table();

private:
	vtable m_vtable;
	vtable m_original_vtable;

};