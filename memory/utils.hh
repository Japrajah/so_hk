#pragma once
#include <Windows.h>
struct utils
{
	static uintptr_t sigscan(const char* pattern, const char* mod = nullptr);
	static __int8* read_address(__int8* address, int offset, int size);
};

