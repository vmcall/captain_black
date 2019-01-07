#pragma once

#include "system_variable.hpp"
#include <Windows.h>

namespace bdo::engine
{
	struct pa_container
	{
		char pad_0000[0x110];			// 000
		system_variable variables[1];	// 110
	};

	class container_wrapper
	{
	public:
		inline container_wrapper(pa_container* container) : m_container(container), m_variables()
		{
			constexpr auto base_offset = offsetof(bdo::engine::pa_container, variables);

			for (auto index = 0; !IsBadReadPtr(this->m_container->variables[index].overload, 1); ++index)
			{
				member current_member = {};
				current_member.offset = static_cast<std::uint64_t>(base_offset) + index * sizeof(bdo::engine::system_variable) + offsetof(bdo::engine::system_variable, value);
				current_member.type = bdo::engine::type::t_int64;

				this->m_variables.emplace(this->m_container->variables[index].name, current_member);
			}
		}

		const variable_array& variables();

		template <class T>
		T* get(const std::string& name)
		{
			return reinterpret_cast<T*>(reinterpret_cast<std::uint64_t>(m_container) + this->m_variables.at(name).offset);
		}

	private:
		variable_array m_variables;
		pa_container* m_container;
	};
}