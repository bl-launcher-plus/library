#pragma once
#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#include "types.h"

/*
* Macros
*/

//Typedef an engine function to use it later
#define BLFUNC(returnType, convention, name, ...)         \
	typedef returnType (convention*name##Fn)(__VA_ARGS__); \
	name##Fn name;

//Typedef a static engine function to use it later
#define BLFUNC_STATIC(returnType, convention, name, ...)         \
	typedef returnType (convention*name##Fn)(__VA_ARGS__); \
	static name##Fn name;

//Typedef an exported engine function to use it later
#define BLFUNC_EXTERN(returnType, convention, name, ...)  \
	typedef returnType (convention*name##Fn)(__VA_ARGS__); \
	extern name##Fn name;

/*
* Engine function declarations
*/

BLFUNC_EXTERN(void, , Printf, const char* format, ...);

//Various definitions for Torque functions that we grab early on.
BLFUNC_EXTERN(bool, , initGame, int argc, const char **argv);
BLFUNC_EXTERN(Namespace *, , LookupNamespace, const char *ns);
BLFUNC_EXTERN(const char *, __thiscall, StringTableInsert, void * stringTablePtr, const char* val, const bool caseSensitive)
BLFUNC_EXTERN(Namespace::Entry *, __thiscall, Namespace__lookup, Namespace *this_, const char *name)
BLFUNC_EXTERN(const char *, __thiscall, CodeBlock__exec, void *this_, U32 offset, Namespace *ns, const char *fnName, U32 argc, const char **argv, bool noCalls, const char *packageName, int setFrame)
BLFUNC_EXTERN(SimObject *, , Sim__findObject_name, const char *name);
BLFUNC_EXTERN(SimObject *, , Sim__findObject_id, unsigned int id);
BLFUNC_EXTERN(unsigned int, , Sim__postEvent, SimObject *destObject, SimEvent *event, U32 time);
BLFUNC_EXTERN(void, , Sim__cancelEvent, unsigned int eventSequence);
BLFUNC_EXTERN(void, __thiscall, SimObject__setDataField, SimObject *this_, const char *name, const char *arr, const char *value)
BLFUNC_EXTERN(const char *, __thiscall, SimObject__getDataField, SimObject *this_, const char *name, const char *arr);
BLFUNC_EXTERN(bool, __thiscall, SimObject__registerObject, SimObject *this_);
BLFUNC_EXTERN(void, __thiscall, SimObject__registerReference, SimObject *this_, SimObject **ptr);
BLFUNC_EXTERN(void, __thiscall, SimObject__unregisterReference, SimObject *this_, SimObject **ptr);
BLFUNC_EXTERN(ConsoleObject *, , AbstractClassRep_create_className, const char *className);
BLFUNC_EXTERN(void, , Sim__init, void);

BLFUNC_EXTERN(void, , SetGlobalVariable, const char *name, const char *value);
BLFUNC_EXTERN(char *, , GetGlobalVariable, const char *name);

BLFUNC_EXTERN(void, __thiscall, ShapeNameHudOnRender, void *, unsigned long, unsigned long, unsigned long);

#endif // FUNCTIONS_H
