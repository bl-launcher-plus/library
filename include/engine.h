#pragma once
#ifndef ENGINE_H
#define ENGINE_H

#include <string>
#include <unordered_map>
#include <memory>

#include "bloader.h"

class Engine
{
public:
	// Engine initialization and destruction
	bool init(std::shared_ptr<class TorqueEngine> torque);
	bool destroy();

	// Module loading and unloading
	int loadModule(const std::string & name);
	int unloadModule(const std::string & name);

	// Module handling
	size_t countModules() const;
	blmodule * getModule(int i) const;
	blmodule * getModule(const char * name) const;
	blmodule * getModule(const std::string & name) const;
	int moduleExist(const std::string & name) const;
	int moduleLoaded(const std::string & name) const;
	int moduleLoaded(const blmodule * module) const;
	const blinfo * const getModuleInfo(const blmodule * module) const;

	// Console functionalities
	void consoleFunction(const char * nameSpace, const char * name,
		bl_callback_void callback, const char * usage, int minArgs, int maxArgs);
	void consoleFunction(const char * nameSpace, const char * name,
		bl_callback_bool callback, const char * usage, int minArgs, int maxArgs);
	void consoleFunction(const char * nameSpace, const char * name,
		bl_callback_int callback, const char * usage, int minArgs, int maxArgs);
	void consoleFunction(const char * nameSpace, const char * name,
		bl_callback_float callback, const char * usage, int minArgs, int maxArgs);
	void consoleFunction(const char * nameSpace, const char * name,
		bl_callback_string callback, const char * usage, int minArgs, int maxArgs);
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

	int printf(const char * format, ...);
	int vprintf(const char * format, void * list);
	int info(const char * format, ...);
	int vinfo(const char * format, void * list);
	int warn(const char * format, ...);
	int vwarn(const char * format, void * list);
	int error(const char * format, ...);
	int verror(const char * format, void * list);

	// Library communications
	void * getSymbol(const blmodule * module, const std::string & func) const;

private:
	std::unordered_map<std::string, std::shared_ptr<blmodule>> modules;
	std::string currDir;

	std::shared_ptr<class TorqueEngine> torque;

	bool loadLibraries();
	int loadLibrary(const std::string & path);

	int _vprintf(const char * code, const char * format, void * list);
};

#endif // ENGINE_H
