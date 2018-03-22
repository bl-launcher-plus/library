#include "engine.h"
#include "blmodule.h"
#include "blibrary.h"
#include "filesystem.h"

#include "torque/torque.h"

#include <algorithm>
#include <cstring>
#include <stdarg.h>

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

bool Engine::init(std::shared_ptr<class TorqueEngine> _torque)
{
	currDir = Filesystem::getCurrentModuleDirectory();

	torque = _torque;

	// Load all the libraries for now
	// TODO: Either make this optional, or make BLauncher 
	return loadLibraries();
}
bool Engine::destroy()
{
	// Just clear them. No need to unlink the connection as it is already destroyed
	// The rest is handled automatically
	modules.clear();

	torque.reset();
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
	for (const auto & func : module->func_void) {
		const char* ns = func.first.c_str();
		if (_stricmp(ns, "") == 0) {
			torque->ConsoleFunction(NULL, func.second.c_str(), empty_void, "", 0, 0);
		}
		else {
			torque->ConsoleFunction(ns, func.second.c_str(), empty_void, "", 0, 0);
		}
	}
	for (const auto & func : module->func_bool)
		torque->ConsoleFunction(func.first.c_str(), func.second.c_str(), empty_bool, "", 0, 0);
	for (const auto & func : module->func_int)
		torque->ConsoleFunction(func.first.c_str(), func.second.c_str(), empty_int, "", 0, 0);
	for (const auto & func : module->func_float)
		torque->ConsoleFunction(func.first.c_str(), func.second.c_str(), empty_float, "", 0, 0);
	for (const auto & func : module->func_string)
		torque->ConsoleFunction(func.first.c_str(), func.second.c_str(), empty_string, "", 0, 0);

	for (const auto & var : module->var_bool)
		torque->ConsoleVariable(var.c_str(), &default_bool);
	for (const auto & var : module->var_int)
		torque->ConsoleVariable(var.c_str(), &default_int);
	for (const auto & var : module->var_float)
		torque->ConsoleVariable(var.c_str(), &default_float);
	for (const auto & var : module->var_string)
		torque->ConsoleVariable(var.c_str(), default_string);

	// Unload the library

	auto deinit = module->library.function(blibrary_deinit);
	if (deinit) {
		deinit();
	}
	else {
		bloader_printf_error("could not find a deinit function");
	}

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
	std::string path = currDir + std::string("/modules/") + std::string(name) + std::string(".dll");
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
	if (_stricmp(nameSpace, "") == 0) {
		torque->ConsoleFunction(NULL, name, (VoidCallback)callback, usage, minArgs, maxArgs);
	}
	else {
		torque->ConsoleFunction(NULL, name, (VoidCallback)callback, usage, minArgs, maxArgs);
	}
}
void Engine::consoleFunction(blmodule * module, const char * nameSpace, const char * name,
	bl_callback_bool callback, const char * usage, int minArgs, int maxArgs)
{
	if (!module)
		return;
	module->func_void.emplace(std::make_pair(nameSpace, name));
	if (_stricmp(nameSpace, "") == 0) {
		torque->ConsoleFunction(NULL, name, (BoolCallback)callback, usage, minArgs, maxArgs);
	}
	else {
		torque->ConsoleFunction(NULL, name, (BoolCallback)callback, usage, minArgs, maxArgs);
	}
}
void Engine::consoleFunction(blmodule * module, const char * nameSpace, const char * name,
	bl_callback_int callback, const char * usage, int minArgs, int maxArgs)
{
	if (!module)
		return;
	module->func_void.emplace(std::make_pair(nameSpace, name));
	if (_stricmp(nameSpace, "") == 0) {
		torque->ConsoleFunction(NULL, name, (IntCallback)callback, usage, minArgs, maxArgs);
	}
	else {
		torque->ConsoleFunction(NULL, name, (IntCallback)callback, usage, minArgs, maxArgs);
	}
}
void Engine::consoleFunction(blmodule * module, const char * nameSpace, const char * name,
	bl_callback_float callback, const char * usage, int minArgs, int maxArgs)
{
	if (!module)
		return;
	module->func_void.emplace(std::make_pair(nameSpace, name));
	if (_stricmp(nameSpace, "") == 0) {
		torque->ConsoleFunction(NULL, name, (FloatCallback)callback, usage, minArgs, maxArgs);
	}
	else {
		torque->ConsoleFunction(NULL, name, (FloatCallback)callback, usage, minArgs, maxArgs);
	}
}
void Engine::consoleFunction(blmodule * module, const char * nameSpace, const char * name,
	bl_callback_string callback, const char * usage, int minArgs, int maxArgs)
{
	if (!module)
		return;
	module->func_void.emplace(std::make_pair(nameSpace, name));
	if (_stricmp(nameSpace, "") == 0) {
		torque->ConsoleFunction(NULL, name, (StringCallback)callback, usage, minArgs, maxArgs);
	}
	else {
		torque->ConsoleFunction(NULL, name, (StringCallback)callback, usage, minArgs, maxArgs);
	}
}

void Engine::consoleVariable(blmodule * module, const char * name, bool * var)
{
	if (!module)
		return;
	module->var_bool.emplace(name);
	torque->ConsoleVariable(name, var);
}
void Engine::consoleVariable(blmodule * module, const char * name, int * var)
{
	if (!module)
		return;
	module->var_int.emplace(name);
	torque->ConsoleVariable(name, var);
}
void Engine::consoleVariable(blmodule * module, const char * name, float * var)
{
	if (!module)
		return;
	module->var_float.emplace(name);
	torque->ConsoleVariable(name, var);
}
void Engine::consoleVariable(blmodule * module, const char * name, char * var)
{
	if (!module)
		return;
	module->var_string.emplace(name);
	torque->ConsoleVariable(name, var);
}

const char * Engine::getGlobalVariable(const char * name)
{
	return GetGlobalVariable(name);
}
void Engine::setGlobalVariable(const char * name, const char * value)
{
	SetGlobalVariable(name, value);
}

// Print a normal text to the engine
int Engine::printf(const char * format, ...)
{
	va_list list;
	va_start(list, format);
	auto size = vprintf(format, list);
	va_end(list);

	return size;
}
int Engine::vprintf(const char * format, void * _list)
{
	return _vprintf(0, format, _list);
}
// Print out information
int Engine::info(const char * format, ...)
{
	va_list list;
	va_start(list, format);
	auto size = vinfo(format, list);
	va_end(list);

	return size;
}
int Engine::vinfo(const char * format, void * list)
{
	return _vprintf(BLOADER_CONSOLE_INFO, format, list);
}
// Print out warning
int Engine::warn(const char * format, ...)
{
	va_list list;
	va_start(list, format);
	auto size = vwarn(format, list);
	va_end(list);

	return size;
}
int Engine::vwarn(const char * format, void * list)
{
	return _vprintf(BLOADER_CONSOLE_WARN, format, list);
}
// Print out error
int Engine::error(const char * format, ...)
{
	va_list list;
	va_start(list, format);
	auto size = verror(format, list);
	va_end(list);

	return size;
}
int Engine::verror(const char * format, void * list)
{
	return _vprintf(BLOADER_CONSOLE_ERROR, format, list);
}
// Perfectly safe printing of limitless text into TorqueEngine
// What could ever go wrong?
int Engine::_vprintf(const char * code, const char * format, void * _list)
{
	va_list list = (va_list)_list;
	va_list list2;
	va_copy(list2, list);

	// Buffer can be null iff count is 0
	auto err = std::vsnprintf(0, 0, format, list2);

	va_end(list2);

	if (err < 0)
		return err;
	std::size_t size = err;
	std::size_t code_size = code ? std::strlen(code) : 0;

	// Prepare buffer
	char * output = new char[size + 1];
	output[size] = 0;

	// Put everything into the buffer
	std::vsnprintf(output, size + 1, format, list);

	// Print out in parts to avoid overflow (Torque limit is 4096)
	char buffer[4096] = { 0 };
	std::size_t max = sizeof(buffer) - 1;
	for (std::size_t i = 0; i < size; i += max - code_size)
	{
		// MS shit
		if (code_size)
		{
			strncpy_s(buffer, sizeof(buffer), code, code_size);
			size += code_size; // For each line of extra code string, add more
		}
		// Get size of 
		auto cp = (std::min)(max, size - i);
		strncpy_s(buffer + code_size, sizeof(buffer) - code_size, output + i, cp - code_size);
		// Make sure last one is nullified
		// Note: Current size ensures that this is indeed the case for each iteration
		buffer[cp] = 0;
		Printf(buffer);
	}

	delete output;

	return size;
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
	std::string path(currDir + "/modules");
	Filesystem folder;
	// Load
	if (!folder.load(path))
	{
		warn("BLoader: Unable to locate modules folder '%s', creating it", path.c_str());
		// Create
		if (!Filesystem::createFolder(path))
		{
			error("BLoader: Unable to create modules folder");
			return false;
		}
		// Load again
		else if (!folder.load(path))
		{
			error("BLoader: Unable to load modules folder");
			return false;
		}
	}

	// Iterate all files
	info("BLoader: loading modules.");

	for (; folder.valid(); folder.next())
	{
		auto module = folder.get();

		// We need to make sure it is a dll
		// TODO: Maybe add some more robust system?
		auto extp = module.find_last_of(".");
		if (lowerString(module.substr(extp + 1)) != "dll")
		{
			warn("BLoader: skipping %s because non-dll.", module.c_str());
			continue;
		}

		module = module.substr(0, extp);
		info("BLoader: loading %s", module.c_str());

		// Got the name, so load as module instead of library
		int status = loadModule(module);
		if (status != BL_OK)
		{
			error("BLoader: Failed to load %s.", module.c_str());
			warn("Reason: %s", bloader_getError(status));
		}
	}

	folder.close();
	return true;
}

int Engine::loadLibrary(const std::string & name)
{
	if (name.empty())
		return BL_INVALID_POINTER;

	std::string path = currDir + std::string("/modules/") + std::string(name) + std::string(".dll");

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
	if (init(module.get()) != 0)
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
