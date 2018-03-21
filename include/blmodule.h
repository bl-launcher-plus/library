#pragma once
#ifndef BLMODULE_H
#define BLMODULE_H

#include <unordered_set>
#include <string>
#include <cstring>

#include "blibrary.h"
#include "library.h"

/*
 * Real implementation of blmodule
 */
struct BLmodule
{
	BLmodule()
	{
		// Note: This is not that secure, but it works
		memset(&info, 0, sizeof(info));
		memset(&library, 0, sizeof(library));
	}

	BLinfo info;

	Library library;

private:
	// Keep track of current implementations
	struct pair_string_hash
	{
		inline std::size_t operator()(const std::pair<std::string, std::string> & v) const
		{
			return std::hash<std::string>()(v.first + "::" + v.second);
		}
	};
public:
	typedef std::unordered_set<std::pair<std::string, std::string>, pair_string_hash> FunctionTable;
	typedef std::unordered_set<std::string> VariableTable;

	FunctionTable func_void;
	FunctionTable func_bool;
	FunctionTable func_int;
	FunctionTable func_float;
	FunctionTable func_string;
	VariableTable var_bool;
	VariableTable var_int;
	VariableTable var_float;
	VariableTable var_string;
};

#endif // BLMODULE_H
