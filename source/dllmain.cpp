#include <windows.h>
#include "Torque.h"
#include "detours.h"

// Direct reference to bloader.cpp
bool bloader_init();
bool bloader_destroy();

MologieDetours::Detour<Sim__initFn> *Sim__init_detour;

void Sim__init_hook(void) {
	bloader_init();

	return Sim__init_detour->GetOriginalFunction()();
}

bool dll_unload() {
	delete Sim__init_detour;
	return bloader_destroy();
}

// Default dll callback
BOOL WINAPI DllMain(HINSTANCE instance, DWORD reason, LPVOID reserved)
{
	if (reason == DLL_PROCESS_ATTACH)
	{
		DisableThreadLibraryCalls(instance);
		if (torque_init()) {
			Sim__init_detour = new MologieDetours::Detour<Sim__initFn>(Sim__init, Sim__init_hook);
		}
		else {
			return false;
		}
		return true;
	}
	else if (reason == DLL_PROCESS_DETACH)
		return dll_unload();

	return TRUE;
}

extern "C" void __declspec(dllexport) __cdecl bloader() {}