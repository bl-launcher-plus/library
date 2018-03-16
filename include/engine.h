#pragma once
#ifndef ENGINE_H
#define ENGINE_H

#include <string>
#include <unordered_map>
#include <memory>

#include "bloader.h"

struct blmodule_internal;

class Engine
{
public:
	// Engine initialization and destruction
	bool init();
	bool destroy();

	// Module loading and unloading
	int loadModule(const std::string & name);
	int unloadModule(const std::string & name);

	// Module handling
	size_t countModules() const;
	const blmodule * getModule(int i) const;
	const blmodule * getModule(const std::string & name) const;
	int moduleExist(const blmodule * module) const;

	// Console funcitonalities
	void consoleFunction(blmodule * module, const char * nameSpace, const char * name,
		bl_callback_void callback, const char * usage, int minArgs, int maxArgs);
	void consoleFunction(blmodule * module, const char * nameSpace, const char * name,
		bl_callback_bool callback, const char * usage, int minArgs, int maxArgs);
	void consoleFunction(blmodule * module, const char * nameSpace, const char * name,
		bl_callback_int callback, const char * usage, int minArgs, int maxArgs);
	void consoleFunction(blmodule * module, const char * nameSpace, const char * name,
		bl_callback_float callback, const char * usage, int minArgs, int maxArgs);
	void consoleFunction(blmodule * module, const char * nameSpace, const char * name,
		bl_callback_string callback, const char * usage, int minArgs, int maxArgs);

	void consoleVariable(blmodule * module, const char * name, bool * var);
	void consoleVariable(blmodule * module, const char * name, int * var);
	void consoleVariable(blmodule * module, const char * name, float * var);
	void consoleVariable(blmodule * module, const char * name, char * var);

	const char * getGlobalVariable(const char * name);
	void setGlobalVariable(const char * name, const char * value);

	// Library communications
	void * getSymbol(const blmodule * module, const std::string & func) const;

private:
	std::unordered_map<std::string, std::shared_ptr<blmodule_internal>> modules;

	int loadLibrary(const std::string & path);
};

#endif // ENGINE_H