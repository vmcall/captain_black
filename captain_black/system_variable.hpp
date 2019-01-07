#pragma once
#include <map>

namespace bdo::engine
{
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
}