#pragma once
#ifndef BLMODULE_H
#define BLMODULE_H

#include <unordered_set>
#include <string>

#include "bloader.h"
#include "library.h"

/*
 * Real implementation of blmodule
 */
struct blmodule_internal : public blmodule
{
	blmodule_internal()
	{
		memset(name, 0, sizeof(name));
		version = 0;
	}
	Library library;

	// Keep track of current implementations
	struct pair_string_hash
	{
		inline std::size_t operator()(const std::pair<std::string, std::string> & v) const
		{
			return std::hash<std::string>()(v.first + "::" + v.second);
		}
	};
	std::unordered_set<std::pair<std::string, std::string>, pair_string_hash> func_void;
	std::unordered_set<std::pair<std::string, std::string>, pair_string_hash> func_bool;
	std::unordered_set<std::pair<std::string, std::string>, pair_string_hash> func_int;
	std::unordered_set<std::pair<std::string, std::string>, pair_string_hash> func_float;
	std::unordered_set<std::pair<std::string, std::string>, pair_string_hash> func_string;
	std::unordered_set<std::string> var_bool;
	std::unordered_set<std::string> var_int;
	std::unordered_set<std::string> var_float;
	std::unordered_set<std::string> var_string;
};

#endif // BLMODULE_H
