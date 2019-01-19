#pragma once
#include <cstdint>
#include <memory>
#include <windows.h>
#include <map>

class vtable_hook
{
public:
	vtable_hook(std::uintptr_t class_base);

private:
	std::uintptr_t m_class_base;
	std::size_t m_size;

};