#pragma once
#include <string>

namespace logger
{
	void initialise();
	void release();
	void log(std::string_view message);
	void log_pointer(std::string_view pointer_name, void* pointer);
};