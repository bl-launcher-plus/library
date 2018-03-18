#include <windows.h>
#include "Torque.h"
#include "detours.h"

// Direct reference to bloader.cpp
bool bloader_init();
bool bloader_destroy();

// TODO: Somehow move this somewhere else where it wont be in the way

// A detour squeezed between a call to their function and their function for our function
MologieDetours::Detour<Sim__initFn> *Sim__init_detour;

// Initialization of Sim
void Sim__init_hook(void)
{
	// This is loaded here to make sure no output is made within the DLL
	// before it is actually loaded.
	bloader_init();

	// All the original function
	return Sim__init_detour->GetOriginalFunction()();
}

// DLL is unloaded
bool dll_unload() {
	delete Sim__init_detour;
	return bloader_destroy();
}

// Default DLL callback
extern "C" __declspec(dllexport) BOOL WINAPI DllMain(HINSTANCE instance, DWORD reason, LPVOID reserved)
{
	if (reason == DLL_PROCESS_ATTACH)
	{
		DisableThreadLibraryCalls(instance);

		// Initialize torque with all what it means
		if (!torque_init())
			return FALSE;

		// Squeeze in our function
		Sim__init_detour = new MologieDetours::Detour<Sim__initFn>(Sim__init, Sim__init_hook);
	}
	else if (reason == DLL_PROCESS_DETACH)
		return dll_unload();

	return TRUE;
}
