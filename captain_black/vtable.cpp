#include "vtable.hpp"
#include <windows.h>

vtable::vtable(std::uintptr_t class_base) : m_class_base(class_base)
{
	std::size_t vmt_size = 0x00;
	auto function_list = *reinterpret_cast<std::uintptr_t***>(class_base);

	// CALCULATE SIZE
	for (; 
		 !IsBadReadPtr(function_list[vmt_size], 1); 
		 vmt_size++) {  }
	
	// RESERVE SIZE IN VECTOR
	this->entries().reserve(vmt_size);

	// CACHE FUNCTIONS
	for (size_t i = 0; i < vmt_size; i++)
	{
		this->entries().emplace_back(&function_list[i]);
		this->functions().emplace_back(function_list[i]);
	}
}

vtable::operator bool() const noexcept
{
	return this->m_class_base != 0x00 && this->m_entries.size() > 0;
}

void** vtable::get(const std::int32_t index) const noexcept
{
	return this->m_entries[index];
}

vtable::entry_list_t& vtable::entries() noexcept
{
	return this->m_entries;
}

vtable::entry_list_t& vtable::functions() noexcept
{
	return this->m_functions;
}

std::uintptr_t& vtable::class_base() noexcept
{
	return this->m_class_base;
}

std::size_t vtable::size() noexcept
{
	return this->entries().size();
}
