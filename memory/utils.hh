#pragma once

struct utils
{
	static __int8* sigscan(const char* pattern, const char* mod);
	static __int8* read_address(__int8* address, int offset, int size);
};

