#include "bloader.h"
#include "Torque.h"
#include "engine.h"

Engine g_engine;

// Initializating engine
bool bloader_init()
{
	if(g_engine.init()) {
		Printf("BLoader %s loaded successfully.", bloader_versionString());
		return true;
	}
	else {
		Printf("BLoader failed to initialize.");
		return false;
	}
}

// Destroying everything
bool bloader_destroy()
{
	if(g_engine.destroy()) {
		Printf("BLoader %s unloaded successfully.", bloader_versionString());
		return true;
	}
	else {
		Printf("Bloader failed to unload.");
		return false;
	}
}


int bloader_version()
{
	return BLOADER_VERSION;
}

const char* bloader_versionString() {
	return BLOADER_VERSION_STRING;
}

const char * bloader_getError(int errorcode)
{
	switch (errorcode)
	{
	// BL_OK
	case BLOADER_OK: return "";
	case BLOADER_NO_MODULE: return "There is no such module";
	case BLOADER_NO_INIT: return "There is no init function in this module";
	case BLOADER_NO_DEINIT: return "There is no deinit function in this module";
	case BLOADER_FAIL_INIT: return "Initialization of module failed";
	case BLOADER_FAIL_DEINIT: return "Deinitialization of module failed";
	case BL_INVALID_POINTER: return "Invalid pointer";
	case BL_INVALID_LIBRARY: return "Invalid module";
	case BL_INVALID_VERSION: return "Invalid version";
	case BL_MISSING_INITIALIZE: return "No initialize found in this module";
	case BL_LIBRARY_INIT_ERROR: return "Error when initializating module";
	case BL_LIBRARY_NO_NAME: return "Module got no valid name";
	default: return "Unknown error";
	}
}

int bloader_load(const char * name)
{
	if (!name)
		return BL_INVALID_POINTER;
	return g_engine.loadModule(name);
}

int bloader_unload(const char * name)
{
	if (!name)
		return BL_INVALID_POINTER;
	return g_engine.unloadModule(name);
}


int bloader_module_count()
{
	return g_engine.countModules();
}

blmodule * bloader_module_fromIndex(int i)
{
	return g_engine.getModule(i);
}

blmodule * bloader_module_fromName(const char * name)
{
	if (!name)
		return nullptr;
	return g_engine.getModule(name);
}

int bloader_module_exist(const char * name)
{
	return g_engine.moduleExist(name);
}

int bloader_module_loaded(const blmodule * module)
{
	return g_engine.moduleLoaded(module);
}

const blinfo * const bloader_module_info(const blmodule * module)
{
	if (!module)
		return nullptr;
	return g_engine.getModuleInfo(module);
}

void bloader_consolefunction_void(blmodule * module, const char * nameSpace, const char * name,
	bl_callback_void callback, const char * usage, int minArgs, int maxArgs)
{
	g_engine.consoleFunction(module, nameSpace, name, callback, usage, minArgs, maxArgs);
}

void bloader_consolefunction_bool(blmodule * module, const char * nameSpace, const char * name,
	bl_callback_bool callback, const char * usage, int minArgs, int maxArgs)
{
	g_engine.consoleFunction(module, nameSpace, name, callback, usage, minArgs, maxArgs);
}

void bloader_consolefunction_int(blmodule * module, const char * nameSpace, const char * name,
	bl_callback_int callback, const char * usage, int minArgs, int maxArgs)
{
	g_engine.consoleFunction(module, nameSpace, name, callback, usage, minArgs, maxArgs);
}

void bloader_consolefunction_float(blmodule * module, const char * nameSpace, const char * name,
	bl_callback_float callback, const char * usage, int minArgs, int maxArgs)
{
	g_engine.consoleFunction(module, nameSpace, name, callback, usage, minArgs, maxArgs);
}

void bloader_consolefunction_string(blmodule * module, const char * nameSpace, const char * name,
	bl_callback_string callback, const char * usage, int minArgs, int maxArgs)
{
	g_engine.consoleFunction(module, nameSpace, name, callback, usage, minArgs, maxArgs);
}

void bloader_consolevariable_bool(blmodule * module, const char * name, int * var)
{
	g_engine.consoleVariable(module, name, (bool *)var);
}

void bloader_consolevariable_int(blmodule * module, const char * name, int * var)
{
	g_engine.consoleVariable(module, name, var);
}

void bloader_consolevariable_float(blmodule * module, const char * name, float * var)
{
	g_engine.consoleVariable(module, name, var);
}

void bloader_consolevariable_string(blmodule * module, const char * name, char * var)
{
	g_engine.consoleVariable(module, name, var);
}

const char * bloader_getVariable(const char * name)
{
	return g_engine.getGlobalVariable(name);
}

void bloader_setVariable(const char * name, const char * value)
{
	g_engine.setGlobalVariable(name, value);
}

void * bloader_symbol(const blmodule * module, const char * func)
{
	if (!module || !func)
		return nullptr;
	return g_engine.getSymbol(module, func);
}
