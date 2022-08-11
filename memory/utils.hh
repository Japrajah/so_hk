#pragma once
#include <Windows.h>
struct utils
{
	static uintptr_t sigscan(const char* pattern, const char* mod = nullptr);
	static uintptr_t read_address(uintptr_t address, int offset, int size);
};

