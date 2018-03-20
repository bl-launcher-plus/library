#include "scanner.h"
#include <windows.h>
#include <psapi.h>

struct scanner_impl
{
	DWORD imageBase;
	DWORD imageSize;

	static bool compareData(PBYTE data, PBYTE pattern, const char * mask);
	static DWORD findPattern(DWORD imageBase, DWORD imageSize, PBYTE pattern, const char* mask);
	DWORD scanFunc(const char* pattern, const char* mask);
	void patchByte(BYTE* location, BYTE value);
};

Scanner::Scanner()
{
	init(nullptr);
}

Scanner::Scanner(const std::string & file)
{
	init(file.c_str());
}

void Scanner::init(const char * file)
{
	impl = std::make_shared<scanner_impl>();

	// Find the module
	auto module = GetModuleHandleA(file);

	if (!module)
		return;

	MODULEINFO info;
	if (!GetModuleInformation(GetCurrentProcess(), module, &info, sizeof(MODULEINFO)))
		return;

	impl->imageBase = reinterpret_cast<DWORD>(info.lpBaseOfDll);
	impl->imageSize = info.SizeOfImage;
}

// Find function and return pointer
void * Scanner::findFunction(const char * pattern, const char * mask)
{
	return reinterpret_cast<void *>(impl->scanFunc(pattern, mask));
}

// Compare data at two locations for equality
bool scanner_impl::compareData(PBYTE data, PBYTE pattern, const char * mask)
{
	// Iterate over the data, pattern and mask in parallel
	for (; *mask; ++data, ++pattern, ++mask)
	{
		// And check for equality at each unmasked byte
		if (*mask == 'x' && *data != *pattern)
			return false;
	}

	return (*mask) == 0;
}

// Find a pattern in memory
DWORD scanner_impl::findPattern(DWORD imageBase, DWORD imageSize, PBYTE pattern, const char* mask)
{
	// Iterate over the image
	for (DWORD i = imageBase; i < imageBase + imageSize; i++)
	{
		// And check for matching pattern at every byte
		if (compareData((PBYTE)i, pattern, mask))
			return i;
	}

	return 0;
}

// Scan the module for a pattern
DWORD scanner_impl::scanFunc(const char* pattern, const char* mask)
{
	// Just search for the pattern in the module
	return findPattern(imageBase, imageSize - strlen(mask), (PBYTE)pattern, mask);
}

//Change a byte at a specific location in memory
void scanner_impl::patchByte(BYTE* location, BYTE value)
{
	//Remove protection
	DWORD oldProtection;
	VirtualProtect(location, 1, PAGE_EXECUTE_READWRITE, &oldProtection);

	//Change value
	*location = value;

	//Restore protection
	VirtualProtect(location, 1, oldProtection, &oldProtection);
}

