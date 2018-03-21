#pragma once
#ifndef TORQUE_H
#define TORQUE_H

#include "scanner.h"
#include "functions.h"

#include <map>
#include <string>

/*
 * Torque Engine wrapper
 */
class TorqueEngine
{
public:
	bool init();

	// Utility functions for looking up, and calling a TorqueScript function.
	Namespace::Entry * fastLookup(const char * ourNamespace, const char * name);
	void* ts__fastCall(Namespace::Entry * ourCall, SimObject* obj, unsigned argc, ...);
	Namespace::Entry * passThroughLookup(Namespace * ns, const char * name);

	void ConsoleFunction(const char * scope, const char * name, StringCallback callBack, const char * usage, int minArgs, int maxArgs);
	void ConsoleFunction(const char * scope, const char * name, IntCallback callBack, const char * usage, int minArgs, int maxArgs);
	void ConsoleFunction(const char * scope, const char * name, FloatCallback callBack, const char * usage, int minArgs, int maxArgs);
	void ConsoleFunction(const char * scope, const char * name, VoidCallback callBack, const char * usage, int minArgs, int maxArgs);
	void ConsoleFunction(const char * scope, const char * name, BoolCallback callBack, const char * usage, int minArgs, int maxArgs);

	void ConsoleVariable(const char *name, int *data);
	void ConsoleVariable(const char *name, bool *data);
	void ConsoleVariable(const char *name, float *data);
	void ConsoleVariable(const char *name, char *data);

	// Evaluate a torquescript string in global scope
	const char * Eval(const char * str);

	const char * StringTableEntry(const char * str, bool caseSensitive = false);

private:

	Scanner scanner;

	// These std::maps are used to cache Namespace::Entrys/Namespaces. 
	// This is done to avoid as many calls to Torque as possible.
	std::map<std::string, Namespace::Entry*> cache;
	std::map<const char*, Namespace*> nscache;
	Namespace* GlobalNS = nullptr;

	void ** StringTable = nullptr; // StringTable pointer
	void * GlobalVars = nullptr; // global variable dictionary pointer

	// Namespace::addCommand overloads
	BLFUNC(void, __thiscall, AddStringCommand, Namespace *ns, const char* name, StringCallback cb, const char *usage, int minArgs, int maxArgs);
	BLFUNC(void, __thiscall, AddIntCommand, Namespace *ns, const char* name, IntCallback cb, const char *usage, int minArgs, int maxArgs);
	BLFUNC(void, __thiscall, AddFloatCommand, Namespace *ns, const char* name, FloatCallback cb, const char *usage, int minArgs, int maxArgs);
	BLFUNC(void, __thiscall, AddVoidCommand, Namespace *ns, const char* name, VoidCallback cb, const char *usage, int minArgs, int maxArgs);
	BLFUNC(void, __thiscall, AddBoolCommand, Namespace *ns, const char* name, BoolCallback cb, const char *usage, int minArgs, int maxArgs);

	// Exposing variables to torquescript
	BLFUNC(void, __thiscall, AddVariable, void * dictionaryPtr, const char* name, int type, void* data);

	// Executing code and calling torquescript functions
	BLFUNC(const char*, , Evaluate, const char* string, bool echo, const char* fileName);

	void rewrite__fatal();
};

#endif // TORQUE_H
