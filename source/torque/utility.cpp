#include "torque.h"

#include <windows.h>
#include <stdarg.h>

void TorqueEngine::rewrite__fatal() {
	Printf("!!! THIS SHOULD NEVER HAPPEN !!!");
}

Namespace::Entry* TorqueEngine::passThroughLookup(Namespace* ns, const char* name) {
	Namespace::Entry* entry;
	std::map<std::string, Namespace::Entry*>::iterator it;

	std::string lol(ns->mName);
	lol.append("__");
	lol.append(name);
	//Identifier* lol = new Identifier();
	it = cache.find(lol); //Look into our Namespace::Entry* cache..
	if (it != cache.end()) {
		entry = it->second;
		if (entry == nullptr) {
			rewrite__fatal();
			Printf("Fatal: found nullptr in cache!");
			cache.erase(it); //Erase it so we don't encounter it again.
			return nullptr;
		}
	}
	else {
		entry = Namespace__lookup(ns, name);
		if (entry == nullptr) {
			//Printf("Could not find function.");
			return nullptr;
		}
		cache.insert(cache.end(), std::make_pair(lol, entry)); //Insert it so further calls are optimized.
	}
	return entry;
}

Namespace::Entry* TorqueEngine::fastLookup(const char* ourNamespace, const char* name) {
	Namespace* ns;
	Namespace::Entry* entry;

	if (_stricmp(ourNamespace, "") == 0) { //If the namespace is blank, assume we're looking in the global namespace.
		if (GlobalNS == nullptr) {
			GlobalNS = LookupNamespace(nullptr);
		}
		ns = GlobalNS;
	}
	else {
		std::map<const char*, Namespace*>::iterator its;
		its = nscache.find(StringTableEntry(ourNamespace));
		if (its != nscache.end()) {
			ns = its->second;
			if (ns == nullptr) {
				//Somehow it got nullptr'd..
				ns = LookupNamespace(ourNamespace);
				if (ns == nullptr) {
					//THIS SHOULD NEVER HAPPEN!
					rewrite__fatal();
					Printf("Fatal: Found cached NS entry with nullptr, could not find namespace!");
					nscache.erase(its);
					return nullptr;
				}
				nscache.erase(its);
				nscache.insert(nscache.end(), std::make_pair(StringTableEntry(ourNamespace), ns));
			}
		}
		else {
			ns = LookupNamespace(StringTableEntry(ourNamespace));
			if (ns == nullptr) {
				rewrite__fatal();
				Printf("Fatal: fastLookup FAILED (2/2)!");
				return nullptr;
			}
			nscache.insert(nscache.end(), std::make_pair(StringTableEntry(ourNamespace), ns));
		}
	}

	std::map<std::string, Namespace::Entry*>::iterator it;
	std::string lol(ourNamespace);
	lol.append("__");
	lol.append(name);
	it = cache.find(lol); //Look into our Namespace::Entry* cache..
	if (it != cache.end()) {
		entry = it->second;
		if (entry == nullptr) {
			rewrite__fatal();
			Printf("Fatal: found nullptr in cache!");
			cache.erase(it); //Erase it so we don't encounter it again.
			return nullptr;
		}
	}
	else {
		entry = Namespace__lookup(ns, StringTableEntry(name));
		if (entry == nullptr) {
			//Printf("Could not find function.");
			return nullptr;
		}
		cache.insert(cache.end(), std::make_pair(lol, entry)); //Insert it so further calls are optimized.
	}

	return entry;
}

void* TorqueEngine::ts__fastCall(Namespace::Entry* ourCall, SimObject* obj, unsigned argc, ...) {
	if (ourCall == nullptr) {
		Printf("Invalid entry.");
		return nullptr;
	}
	if (argc > 19) {
		Printf("Too many arguments for torquescript to handle..");
		return nullptr;
	}
	const char* argv[21];
	va_list args;
	va_start(args, argc);
	argv[0] = ourCall->mFunctionName;
	for (unsigned i = 0; i < argc; i++) {
		argv[i + 1] = va_arg(args, const char*);
	}
	argc++;
	va_end(args);
	if (ourCall->mType == Namespace::Entry::ScriptFunctionType) {
		if (ourCall->mFunctionOffset) {
			const char* retVal = CodeBlock__exec(
				ourCall->mCode, ourCall->mFunctionOffset,
				ourCall->mNamespace, ourCall->mFunctionName,
				argc, argv, false, ourCall->mNamespace->mPackage,
				0);
			return (void*)retVal; //we know what it's supposed to return.
		}
		else {
			return nullptr;
		}
	}
	U32 mMinArgs = ourCall->mMinArgs, mMaxArgs = ourCall->mMaxArgs;
	if ((mMinArgs && argc < mMinArgs) || (mMaxArgs && argc > mMaxArgs)) {
		Printf("Expected args between %d and %d, got %d", mMinArgs, mMaxArgs, argc);
		return nullptr;
	}
	SimObject* actualObj;
	if (obj == nullptr) {
		actualObj = nullptr;
	}
	else
	{
		actualObj = obj;
	}
	switch (ourCall->mType) {
	case Namespace::Entry::StringCallbackType:
		return (void*)ourCall->cb.mStringCallbackFunc(actualObj, argc, argv);
	case Namespace::Entry::IntCallbackType:
		return (void*)ourCall->cb.mIntCallbackFunc(actualObj, argc, argv);
	case Namespace::Entry::FloatCallbackType: {
		//Wtf?
		// TODO: Figure out why this is a thing and how to fix it
		float ourret[] = { ourCall->cb.mFloatCallbackFunc(actualObj, argc, argv) };
		return (void*)ourret;
	}
	case Namespace::Entry::BoolCallbackType:
		return (void*)ourCall->cb.mBoolCallbackFunc(actualObj, argc, argv);
	case Namespace::Entry::VoidCallbackType:
		ourCall->cb.mVoidCallbackFunc(actualObj, argc, argv);
		return nullptr;
	}

	return nullptr;
}

const char * TorqueEngine::StringTableEntry(const char * str, bool caseSensitive)
{
	// Make sure it is allocated
	if (!*StringTable)
		return "";
	return StringTableInsert(*StringTable, str, caseSensitive);
}

//Register a torquescript function that returns a string. The function must look like this:
//const char* func(DWORD* obj, int argc, const char* argv[])
void TorqueEngine::ConsoleFunction(const char* nameSpace, const char* name, StringCallback callBack, const char* usage, int minArgs, int maxArgs)
{
	AddStringCommand(LookupNamespace(nameSpace), StringTableEntry(name, false), callBack, usage, minArgs, maxArgs);
}

//Register a torquescript function that returns an int. The function must look like this:
//int func(DWORD* obj, int argc, const char* argv[])
void TorqueEngine::ConsoleFunction(const char* nameSpace, const char* name, IntCallback callBack, const char* usage, int minArgs, int maxArgs)
{
	AddIntCommand(LookupNamespace(nameSpace), StringTableEntry(name, false), callBack, usage, minArgs, maxArgs);
}

//Register a torquescript function that returns a float. The function must look like this:
//float func(DWORD* obj, int argc, const char* argv[])
void TorqueEngine::ConsoleFunction(const char* nameSpace, const char* name, FloatCallback callBack, const char* usage, int minArgs, int maxArgs)
{
	AddFloatCommand(LookupNamespace(nameSpace), StringTableEntry(name, false), callBack, usage, minArgs, maxArgs);
}

//Register a torquescript function that returns nothing. The function must look like this:
//void func(DWORD* obj, int argc, const char* argv[])
void TorqueEngine::ConsoleFunction(const char* nameSpace, const char* name, VoidCallback callBack, const char* usage, int minArgs, int maxArgs)
{
	AddVoidCommand(LookupNamespace(nameSpace), StringTableEntry(name, false), callBack, usage, minArgs, maxArgs);
}

//Register a torquescript function that returns a bool. The function must look like this:
//bool func(DWORD* obj, int argc, const char* argv[])
void TorqueEngine::ConsoleFunction(const char* nameSpace, const char* name, BoolCallback callBack, const char* usage, int minArgs, int maxArgs)
{
	AddBoolCommand(LookupNamespace(nameSpace), StringTableEntry(name, false), callBack, usage, minArgs, maxArgs);
}

//Expose an integer variable to torquescript
void TorqueEngine::ConsoleVariable(const char* name, int* data)
{
	AddVariable(GlobalVars, name, 4, data);
}

//Expose a boolean variable to torquescript
void TorqueEngine::ConsoleVariable(const char* name, bool* data)
{
	AddVariable(GlobalVars, name, 6, data);
}

//Expose a float variable to torquescript
void TorqueEngine::ConsoleVariable(const char* name, float* data)
{
	AddVariable(GlobalVars, name, 8, data);
}

//Expose a string variable to torquescript
void TorqueEngine::ConsoleVariable(const char* name, char* data)
{
	AddVariable(GlobalVars, name, 10, data);
}

//Evaluate a torquescript string in global scope
const char* TorqueEngine::Eval(const char* str)
{
	return Evaluate(str, false, nullptr);
}

