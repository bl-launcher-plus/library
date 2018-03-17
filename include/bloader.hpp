#pragma once
#ifndef BLOADER_HPP

#include "bloader.h"

#include <string>
#include <vector>
#include <functional>

/*
 * C++11 class for more convenient access to the API
 * There is no optimizations, so use sparingly
 * It is made completely inline for quick use anywhere
 */
class BLoader
{
public:
	// Get library version
	inline int version() const
	{
		return bloader_version();
	}
	// Get library symatic versioning
	inline std::string versionString() const
	{
		return bloader_versionString();
	}
	// Get last error
	inline std::string getError() const
	{
		return bloader_getError(error);
	}
	// Load a module by name
	inline bool loadModule(const std::string & name)
	{
		error = bloader_load(name.c_str());
		return error == BLOADER_OK;
	}
	// Unload a module by name
	inline bool unloadModule(const std::string & name)
	{
		error = bloader_unload(name.c_str());
		return error == BLOADER_OK;
	}
	// Get all modules in a list
	inline std::vector<blmodule *> getModules()
	{
		auto count = bloader_module_count();
		decltype(getModules()) modules;
		modules.reserve(count);
		for (decltype(count) i = 0; i < count; ++i)
			modules.push_back(bloader_module_fromIndex(i));
		return modules;
	}
	// Get a module by name
	inline blmodule * getModule(const std::string & name)
	{
		return bloader_module_fromName(name.c_str());
	}
	// Check if a module exists
	inline bool moduleExist(const std::string & name)
	{
		error = bloader_module_exist(name.c_str());
		return error == BLOADER_OK;
	}
	// Check if a module is loaded
	inline bool moduleLoaded(const blmodule * module)
	{
		error = bloader_module_loaded(module);
		return error == BLOADER_OK;
	}
	// Register void console function
	inline void registerConsoleFunction(blmodule * module,
		const char * nameSpace, const char * name, bl_callback_void callback,
		const char * usage, int minArgs, int maxArgs)
	{
		bloader_consolefunction_void(module, nameSpace, name,
			callback, usage, minArgs, maxArgs);
	}
	// Register bool console function
	inline void registerConsoleFunction(blmodule * module,
		const char * nameSpace, const char * name, bl_callback_bool callback,
		const char * usage, int minArgs, int maxArgs)
	{
		bloader_consolefunction_bool(module, nameSpace, name,
			callback, usage, minArgs, maxArgs);
	}
	// Register int console function
	inline void registerConsoleFunction(blmodule * module,
		const char * nameSpace, const char * name, bl_callback_int callback,
		const char * usage, int minArgs, int maxArgs)
	{
		bloader_consolefunction_int(module, nameSpace, name,
			callback, usage, minArgs, maxArgs);
	}
	// Register float console function
	inline void registerConsoleFunction(blmodule * module,
		const char * nameSpace, const char * name, bl_callback_float callback,
		const char * usage, int minArgs, int maxArgs)
	{
		bloader_consolefunction_float(module, nameSpace, name,
			callback, usage, minArgs, maxArgs);
	}
	// Register string console function
	inline void registerConsoleFunction(blmodule * module,
		const char * nameSpace, const char * name, bl_callback_string callback,
		const char * usage, int minArgs, int maxArgs)
	{
		bloader_consolefunction_string(module, nameSpace, name,
			callback, usage, minArgs, maxArgs);
	}
	// Register bool console variable
	inline void registerConsoleVariable(blmodule * module, const char * name, bool * var)
	{
		bloader_consolevariable_bool(module, name, (int *)var);
	}
	// Register int console variable
	inline void registerConsoleVariable(blmodule * module, const char * name, int * var)
	{
		bloader_consolevariable_int(module, name, var);
	}
	// Register float console variable
	inline void registerConsoleVariable(blmodule * module, const char * name, float * var)
	{
		bloader_consolevariable_float(module, name, var);
	}
	// Register string console variable
	inline void registerConsoleVariable(blmodule * module, const char * name, char * var)
	{
		bloader_consolevariable_string(module, name, var);
	}
	// Get global variable
	inline std::string getVariable(const std::string & name)
	{
		return bloader_getVariable(name.c_str());
	}
	// Set global variable
	inline void setVariable(const std::string & name, const std::string & value)
	{
		bloader_setVariable(name.c_str(), value.c_str());
	}
	// Set global vraiable, convert it internally
	template<typename T>
	void setVariable(const std::string & name, T value)
	{
		setVariable(name, std::to_string(value));
	}
	// Get symbol from module, converted to type
	template<typename T>
	T * getSymbol(const blmodule * module, const std::string & name)
	{
		return static_cast<T *>(bloader_symbol(module, name.c_str()));
	}
	// Get function from module
	template<typename T>
	std::function<T> getFunctionEx(const blmodule * module, const std::string & name)
	{
		return static_cast<T *>(bloader_symbol(module, name.c_str()));
	}
	// Helper macro to convert function
	#define getFunction(mod, func) getFunctionEx<decltype(func)>(#func)

private:
	// Latest error
	int error = 0;
};

#endif // BLOADER_HPP
