#pragma once
#include <cstdint>
#include <memory>

namespace bdo::engine
{
	struct struct_text_arg
	{
		wchar_t value[4];
		std::int64_t pad08;
		std::int64_t pad10;
		std::int64_t some_size;
		std::int8_t pad[0x50];
	};

	struct command_args
	{
		struct_text_arg *text_argument;
		std::int64_t struct_end;
		std::int8_t pad[0x50];
	};

	class command_helper
	{
	public:
		void reloadui();
		void loot();


		template <std::int64_t function_address, size_t N>
		void* do_command(const wchar_t(&message)[N], std::int32_t pad)
		{
			struct_text_arg text_arg = {};
			text_arg.some_size = 7;
			memcpy(text_arg.value, message, sizeof(message));

			command_args args = {};
			args.text_argument = &text_arg;
			args.struct_end = reinterpret_cast<std::int64_t>(args.text_argument) + pad;

			std::int8_t test_buffer[30] = {};

			using function_t = void*(__fastcall*)(void*, command_args*);
			return reinterpret_cast<function_t>(function_address)(test_buffer, &args);
		}
	};
}
