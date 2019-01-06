#pragma once
#include <cstdint>
#include <map>
#include <windows.h>
#include "logger.hpp"

namespace bdo::engine
{
	constexpr auto speedcap_function_offset = 0x89E1A0;
	constexpr auto local_actor_offset = 0x33AE948;

	struct vector3d
	{
		float x;
		float y;
		float z;
	};

	struct actor
	{
		char pad_0000[104];					// 000
		wchar_t* name;						// 068
		char pad_0070[160];					// 070
		vector3d position;					// 110
		char pad_011C[2780];				// 11C
		std::int32_t movement_speed;		// BF8
		std::int32_t attack_speed;			// BFC
		std::int32_t cast_speed;			// C00
		char pad_0C04[2];					// C04
		std::uint16_t movement_backwards;	// C06
	};


	enum type : std::uint8_t
	{
		t_int8,
		t_int16,
		t_int32,
		t_int64,
		t_float
	};

	struct member
	{
		std::uint64_t offset;
		bdo::engine::type type;
	};

	using variable_array = std::map<std::string, member>;
	struct system_variable
	{
		void* overload; // 0000
		void* reserved; // 0008
		void* value;	// 0010
		char* name;		// 0018
	};


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