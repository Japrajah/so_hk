#include "utils.hh"
#include <vector>


constexpr int m_strlen(const char* str)
{
	int cnt = 0;
	if (!str)
		return 0;
	for (; *str != '\0'; ++str)
		++cnt;
	return cnt;
}
constexpr bool m_isspace(char symbol)
{
	return (symbol == ' ' || symbol == '\n' || symbol == '\t' || symbol == '\v' || symbol == '\f' || symbol == '\r');
}
constexpr int m_isdigit(unsigned char c)
{
	return (c >= '0' && c <= '9' ? 1 : 0);
}

constexpr int m_isalpha(unsigned char c)
{
	/*
	 * Depends on ASCII-like character ordering.
	 */
	return ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') ? 1 : 0);
}


constexpr char m_isupper(unsigned char c)
{
	if (c >= (unsigned char)('A') && c <= (unsigned char)('Z'))
		return 1;
	return 0;
}
constexpr unsigned long m_strtoul(const char* nptr, char** endptr, register int base)
{
	register const char* s = nptr;
	register unsigned long acc;
	register int c;
	register unsigned long cutoff;
	register int neg = 0, any, cutlim;
	do {
		c = *s++;
	} while (m_isspace(c));
	if (c == '-') {
		neg = 1;
		c = *s++;
	}
	else if (c == '+')
		c = *s++;
	if ((base == 0 || base == 16) &&
		c == '0' && (*s == 'x' || *s == 'X')) {
		c = s[1];
		s += 2;
		base = 16;
	}
	if (base == 0)
		base = c == '0' ? 8 : 10;
	cutoff = (unsigned long)ULONG_MAX / (unsigned long)base;
	cutlim = (unsigned long)ULONG_MAX % (unsigned long)base;
	for (acc = 0, any = 0;; c = *s++) {
		if (m_isdigit(c))
			c -= '0';
		else if (m_isalpha(c))
			c -= m_isupper(c) ? 'A' - 10 : 'a' - 10;
		else
			break;
		if (c >= base)
			break;
		if (any < 0 || acc > cutoff || (acc == cutoff && c > cutlim))
			any = -1;
		else {
			any = 1;
			acc *= base;
			acc += c;
		}
	}
	if (any < 0) {
		acc = ULONG_MAX;
		//	errno = ERANGE;
	}
	else if (neg)
		acc = -acc;
	if (endptr != 0)
		*endptr = (char*)(any ? s - 1 : nptr);
	return (acc);
}

const auto pattern_to_byte(const char* pattern)
{
	
	std::vector<int> bytesmm{};
		const auto startmm = const_cast<char*>(pattern);
		const auto endmm = const_cast<char*>(pattern) + m_strlen(pattern);

		for (auto currentmm = startmm; currentmm < endmm; ++currentmm)
		{
			if (*currentmm == '?')
			{
				++currentmm;
				if (*currentmm == '?')
					++currentmm;
				bytesmm.push_back(-1);
			}
			else { bytesmm.push_back(m_strtoul(currentmm, &currentmm, 16)); }
		}
		return bytesmm;
}

uintptr_t utils::sigscan(const char* pattern, const char* mod  )
{
	uintptr_t moduleAdressmm = 0;
	moduleAdressmm = (uintptr_t)GetModuleHandleA(mod);
	const auto dosHeadermm = (PIMAGE_DOS_HEADER)moduleAdressmm;
	const auto ntHeadersmm = (PIMAGE_NT_HEADERS)((std::uint8_t*)moduleAdressmm + dosHeadermm->e_lfanew);
	const auto sizeOfImage = ntHeadersmm->OptionalHeader.SizeOfImage;
	const std::vector<int> patternBytesmm  = pattern_to_byte(pattern);
   
	const auto scanBytesmm = reinterpret_cast<std::uint8_t*>(moduleAdressmm);
	const auto smm = patternBytesmm.size();
	const auto dmm = patternBytesmm.data();
	for (auto imm = 0ul; imm < sizeOfImage - smm; ++imm)
	{
		bool foundmm = true;
		for (auto jmm = 0ul; jmm < smm; ++jmm)
		{
			if (scanBytesmm[imm + jmm] != dmm[jmm] && dmm[jmm] != -1)
			{
				foundmm = false;
				break;
			}
		}
		if (foundmm) { return reinterpret_cast<uintptr_t>(&scanBytesmm[imm]); }
	}
	return NULL;
}

uintptr_t utils::read_address(uintptr_t address, int offset, int size) {
	return (address + size + *(int*)(address + offset));
}