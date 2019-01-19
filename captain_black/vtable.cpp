#include "vtable.hpp"

vtable::vtable(std::uintptr_t class_base) : m_class_base(class_base)
{
	// CALCULATE SIZE

	
}

vtable::operator bool() const noexcept
{
	return m_class_base == 0x00;
}
