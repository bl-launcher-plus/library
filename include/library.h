#pragma once
#ifndef LIBRARY_H
#define LIBRARY_H

#include <string>
#include <functional>
#include <memory>

class Library
{
public:
	Library();
	// Load the library
	bool load(const std::string & path);
	// Unload the library
	void unload();
	// Get a symbol
	void * symbol(const std::string & name) const;
	// Get a variable
	template<typename T>
	T * variable(const std::string & name) const
	{
		return (T*)symbol(name);
		//return static_cast<T *>(symbol(name));
	}
	// Get a function
	template<typename T>
	std::function<T> functionex(const std::string & name) const
	{
		return (T*)symbol(name);
		//return static_cast<T *>(symbol(name.c_str()));
	}
	// Helper macro for easier fetching a function
#define function(func) functionex<decltype(func)>(std::string(#func))
	// Check if any error have happened
	// Note: Implementation might clear the result after first call
	bool error() const;
	// Get error message
	// Note: Implementation might clear the result after first call
	std::string errormsg() const;

	// Check if library is loaded
	operator bool() const;
	// Copy over library to other structure
	Library & operator=(const Library & l);
private:
	std::shared_ptr<class lib_impl> impl;
};

#endif // LIBRARY_H
