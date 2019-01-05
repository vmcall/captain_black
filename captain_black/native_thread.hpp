#pragma once
#include <windows.h>

template <auto value>
class native_thread
{
public:
	__forceinline auto thread_handle() -> HANDLE&
	{
		return this->m_thread_handle;
	}

	__forceinline void run(void* args)
	{
		this->thread_handle() = CreateThread(nullptr, 0, reinterpret_cast<LPTHREAD_START_ROUTINE>(value), args, 0, nullptr);
	}

private:
	HANDLE m_thread_handle;
};