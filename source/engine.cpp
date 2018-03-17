#include "engine.h"
#include "blmodule.h"
#include "blibrary.h"
#include "Torque.h"
#include "filesystem.h"

#include <algorithm>

// Empties and defaults
static VoidCallback empty_void = [](SimObject *, int, const char*[]) {};
static BoolCallback empty_bool = [](SimObject *, int, const char*[]) { return false; };
static IntCallback empty_int = [](SimObject *, int, const char*[]) { return 0; };
static FloatCallback empty_float = [](SimObject *, int, const char*[]) { return 0.0f; };
static StringCallback empty_string = [](SimObject *, int, const char*[]) { return ""; };
static bool default_bool = false;
static int default_int = 0;
static float default_float = 0.0f;
static char default_string[4096] = { 0 };

// Safely copy a cstring to string
inline std::string safeCopyString(const char * cstr, int size)
{
	return std::string(cstr, strnlen(cstr, size - 1));
}

// Get size of a member in a structure
#define sizeofmember(clas, mem) (sizeof(clas::mem))

char lowerChar(char in)
{
	if (in >= 'A' && in <= 'Z')
		return in - ('Z' - 'A');
	return in;
}

inline std::string lowerString(const std::string & s)
{
	std::string str = s;
	std::transform(str.begin(), str.begin(), str.begin(), lowerChar);
	return str;
}

bool Engine::init()
{
	// if (!torque_init())
	//	return false;
	// Load all the libraries for now
	// TODO: Either make this optional, or make BLauncher 
	loadLibraries();
	return true;
}
bool Engine::destroy()
{
	// Just clear them. No need to unlink the connection as it is already destroyed
	// The rest is handled automatically
	modules.clear();
	return true;
}

int Engine::loadModule(const std::string & name)
{
	auto it = modules.find(name);
	// Already loaded
	if (it != modules.end())
		return BLOADER_OK;

	return loadLibrary(name);
}

int Engine::unloadModule(const std::string & name)
{
	auto it = modules.find(name);
	// Already loaded
	if (it == modules.end())
		return BLOADER_NO_MODULE;

	auto module = it->second;
	// Make all functions and variables invalid
	for (const auto & func : module->func_void)
		ConsoleFunction(func.first.c_str(), func.second.c_str(), empty_void, "", 0, 0);
	for (const auto & func : module->func_bool)
		ConsoleFunction(func.first.c_str(), func.second.c_str(), empty_bool, "", 0, 0);
	for (const auto & func : module->func_int)
		ConsoleFunction(func.first.c_str(), func.second.c_str(), empty_int, "", 0, 0);
	for (const auto & func : module->func_float)
		ConsoleFunction(func.first.c_str(), func.second.c_str(), empty_float, "", 0, 0);
	for (const auto & func : module->func_string)
		ConsoleFunction(func.first.c_str(), func.second.c_str(), empty_string, "", 0, 0);

	for (const auto & var : module->var_bool)
		ConsoleVariable(var.c_str(), &default_bool);
	for (const auto & var : module->var_int)
		ConsoleVariable(var.c_str(), &default_int);
	for (const auto & var : module->var_float)
		ConsoleVariable(var.c_str(), &default_float);
	for (const auto & var : module->var_string)
		ConsoleVariable(var.c_str(), default_string);

	// Unload the library
	module->library.unload();

	// And remove
	modules.erase(it);

	return BLOADER_OK;
}

size_t Engine::countModules() const
{
	return modules.size();
}

blmodule * Engine::getModule(int i) const
{
	if (i < 0 || (size_t)i >= modules.size())
		return nullptr;
	auto it = modules.begin();
	// Note: O(n) complexity
	std::advance(it, i);
	return it->second.get();
}
blmodule * Engine::getModule(const char * name) const
{
	if (!name)
		return nullptr;
	auto _name = safeCopyString(name, sizeofmember(blinfo, name));
	return getModule(_name);
}
blmodule * Engine::getModule(const std::string & name) const
{
	auto it = modules.find(name);
	if (it == modules.end())
		return nullptr;
	return it->second.get();
}

int Engine::moduleExist(const std::string & name) const
{
	// Find i exist in list already
	auto it = modules.find(name);
	if (it != modules.end())
		return BL_OK;
	// Check if found in filesystem
	std::string path = std::string("modules/") + std::string(name) + std::string(".dll");
	return Filesystem::exists(path) ? BL_OK : BL_INVALID_LIBRARY;
}

int Engine::moduleLoaded(const std::string & name) const
{
	auto it = modules.find(name);
	if (it == modules.end())
		return BL_INVALID_LIBRARY;
	return BL_OK;
}

int Engine::moduleLoaded(const blmodule * module) const
{
	if (!module)
		return BL_INVALID_POINTER;
	auto name = safeCopyString(module->info.name, sizeof(module->info.name));
	auto it = modules.find(name);
	if (it == modules.end())
		return BL_INVALID_LIBRARY;
	if (module->info.version != 0 && it->second->info.version != module->info.version)
		return BL_INVALID_VERSION;
	return BL_OK;
}

const blinfo * const Engine::getModuleInfo(const blmodule * module) const
{
	if (!module)
		return nullptr;
	return &module->info;
}

void Engine::consoleFunction(blmodule * module, const char * nameSpace, const char * name,
	bl_callback_void callback, const char * usage, int minArgs, int maxArgs)
{
	if (!module)
		return;
	module->func_void.emplace(std::make_pair(nameSpace, name));
	ConsoleFunction(nameSpace, name, (VoidCallback)callback, usage, minArgs, maxArgs);
}
void Engine::consoleFunction(blmodule * module, const char * nameSpace, const char * name,
	bl_callback_bool callback, const char * usage, int minArgs, int maxArgs)
{
	if (!module)
		return;
	module->func_void.emplace(std::make_pair(nameSpace, name));
	ConsoleFunction(nameSpace, name, (BoolCallback)callback, usage, minArgs, maxArgs);
}
void Engine::consoleFunction(blmodule * module, const char * nameSpace, const char * name,
	bl_callback_int callback, const char * usage, int minArgs, int maxArgs)
{
	if (!module)
		return;
	module->func_void.emplace(std::make_pair(nameSpace, name));
	ConsoleFunction(nameSpace, name, (IntCallback)callback, usage, minArgs, maxArgs);
}
void Engine::consoleFunction(blmodule * module, const char * nameSpace, const char * name,
	bl_callback_float callback, const char * usage, int minArgs, int maxArgs)
{
	if (!module)
		return;
	module->func_void.emplace(std::make_pair(nameSpace, name));
	ConsoleFunction(nameSpace, name, (FloatCallback)callback, usage, minArgs, maxArgs);
}
void Engine::consoleFunction(blmodule * module, const char * nameSpace, const char * name,
	bl_callback_string callback, const char * usage, int minArgs, int maxArgs)
{
	if (!module)
		return;
	module->func_void.emplace(std::make_pair(nameSpace, name));
	ConsoleFunction(nameSpace, name, (StringCallback)callback, usage, minArgs, maxArgs);
}

void Engine::consoleVariable(blmodule * module, const char * name, bool * var)
{
	if (!module)
		return;
	module->var_bool.emplace(name);
	ConsoleVariable(name, var);
}
void Engine::consoleVariable(blmodule * module, const char * name, int * var)
{
	if (!module)
		return;
	module->var_int.emplace(name);
	ConsoleVariable(name, var);
}
void Engine::consoleVariable(blmodule * module, const char * name, float * var)
{
	if (!module)
		return;
	module->var_float.emplace(name);
	ConsoleVariable(name, var);
}
void Engine::consoleVariable(blmodule * module, const char * name, char * var)
{
	if (!module)
		return;
	module->var_string.emplace(name);
	ConsoleVariable(name, var);
}

const char * Engine::getGlobalVariable(const char * name)
{
	return GetGlobalVariable(name);
}
void Engine::setGlobalVariable(const char * name, const char * value)
{
	SetGlobalVariable(name, value);
}

void * Engine::getSymbol(const blmodule * module, const std::string & func) const
{
	if (!module)
		return nullptr;
	if (!module->library)
		return nullptr;

	return module->library.symbol(func);
}

bool Engine::loadLibraries()
{
	std::string path("modules");
	Filesystem folder;
	// Load
	if (!folder.load(path))
	{
		// Create
		if (!Filesystem::createFolder(path))
			return false;
		// Load again
		else if (!folder.load(path))
			return false;
	}

	// Iterate all files
	for (; folder.valid(); folder.next())
	{
		auto module = folder.get();
		// We need to make sure it is a dll
		// TODO: Maybe add some more robust system?
		auto extp = module.find_last_of(".");
		if (lowerString(module.substr(extp + 1)) != "dll")
			continue;
		module = module.substr(0, extp);

		// Got the name, so load as module instead of library
		loadModule(module);
	}

	folder.close();
	return true;
}

int Engine::loadLibrary(const std::string & name)
{
	if (name.empty())
		return BL_INVALID_POINTER;

	std::string path = std::string("modules/") + std::string(name) + std::string(".dll");

	// Load library
	Library lib;
	if (!lib.load(path))
		return BL_INVALID_LIBRARY;

	// Locate initialization
	auto init = lib.function(blibrary_initialize);
	if (!init)
	{
		lib.unload();
		return BL_MISSING_INITIALIZE;
	}

	// Locate info
	auto info = lib.function(blibrary_info);
	if (!info)
	{
		lib.unload();
		return BL_MISSING_INFO;
	}

	// Allocate module
	auto module = std::make_shared<BLmodule>();
	module->library = lib;

	// Initialize the module
	if (init() != 0)
	{
		lib.unload();
		return BL_LIBRARY_INIT_ERROR;
	}

	// Get the rest of the information
	info(&module->info);

	// Null last byte for those that borked up
	module->info.name[sizeof(module->info.name) - 1] = 0;
	module->info.description[sizeof(module->info.description) - 1] = 0;

	// Put in map
	modules.emplace(name, module);

	return BL_OK;
}
