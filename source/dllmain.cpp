#include <windows.h>

// Direct reference to bloader.cpp
bool bloader_init();
bool bloader_destroy();

// Default dll callback
BOOL WINAPI DllMain(HINSTANCE instance, DWORD reason, LPVOID reserved)
{
	if (reason == DLL_PROCESS_ATTACH)
	{
		DisableThreadLibraryCalls(instance);
		return bloader_init();
	}
	else if (reason == DLL_PROCESS_DETACH)
		return bloader_destroy();

	return TRUE;
}
