#define NOMINMAX
#include "vtable_hook.hpp"

bool vtable_hook::hook(std::int32_t index, void* hook)
{
	// SANITY CHECK
	if (index > this->m_vtable.size())
		return false;

	*this->m_vtable.get(index) = hook;
	return true;
}

bool vtable_hook::restore_table()
{
	for (size_t i = 0; i < this->m_vtable.size(); i++)
	{
		*this->m_vtable.entries().at(i) = this->m_original_vtable.functions().at(i);
	}
}
