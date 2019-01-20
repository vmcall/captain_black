#pragma once

#include <cstdint>
#include <cstddef>
#include <vector>

class vtable
{
public:
	explicit vtable(std::uintptr_t class_base = 0x00) noexcept;
	
	// VALIDITY CHECK
	operator bool() const noexcept;

	template <class fn_t>
	fn_t at(const std::int32_t index) const noexcept
	{
		if (index > m_entries.size())
			return reinterpret_cast<fn_t>(nullptr);
	
		return reinterpret_cast<fn_t>(*m_entries.at(index));
	}

	void** get(const std::int32_t index) const noexcept;

	using entry_list_t = std::vector<void**>;
	vtable::entry_list_t& entries() noexcept;

	using function_list_t = std::vector<void*>;
	vtable::entry_list_t& functions() noexcept;

	std::uintptr_t& class_base() noexcept;

	std::size_t size() noexcept;

private:
	vtable::entry_list_t m_entries;
	vtable::entry_list_t m_functions;
	std::uintptr_t m_class_base;
};