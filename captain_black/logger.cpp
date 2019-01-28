#define NOMINMAX
#include "logger.hpp"
#include <windows.h>
#include <iostream>
#include <string_view> 

void logger::initialise()
{
	// ALLOC CONSOLE FOR DEBUG OUTPUT
	AllocConsole();

#pragma warning(push)
#pragma warning(disable:4996)
	freopen("CONOUT$", "w", stdout);
#pragma warning(pop)

	SetConsoleTitle(L"[CAPTAIN_BLACK]");
}

void logger::release()
{
	// FREE OUR CONSOLE
	ShowWindow(GetConsoleWindow(), SW_HIDE);
	FreeConsole();
}

void logger::log(std::string_view message)
{
	std::cout << "[+] " << message << std::endl;
}

void logger::log_pointer(std::string_view pointer_name, void* pointer)
{
	std::cout << "[?] " << pointer_name << ": " << std::hex << pointer << std::dec << std::endl;
}
