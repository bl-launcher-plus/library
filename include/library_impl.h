#pragma once
#ifndef LIBRARY_IMPL_H

/*
 * Implementation of the Library
 * This is made for direct access if needed
 * lib_impl is most probably the only useful here
 * Other than that, just use Library as it should be as fast as this
 */

#include <string>
#include <functional>
#include <type_traits>

// Windows
#if defined(_WIN32)
#include <windows.h>

typedef decltype(LoadLibraryEx(0, 0, 0)) libtype;

inline auto libload(const std::string & path) -> libtype
{
	return LoadLibraryEx(path.c_str(), NULL, DONT_RESOLVE_DLL_REFERENCES);
}
inline auto libunload(libtype lib)
{
	return FreeLibrary(lib);
}
inline auto libsymbol(libtype lib, const std::string & name) -> decltype(GetProcAddress(0, 0))
{
	return GetProcAddress(lib, name.c_str());
}
inline std::string liberror(libtype)
{
	auto err = GetLastError();
	if (!err)
		return std::string();

	LPSTR msgBuf = nullptr;
	auto size = FormatMessageA(
		FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
		NULL, err, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (LPSTR)&msgBuf, 0, NULL
	);

	std::string msg(msgBuf, size);

	LocalFree(msgBuf);

	return msg;
}
inline void libreset(libtype)
{
	GetLastError();
}

// Linux, Mac
#elif defined(__unix__)
#include <dlfcn.h> // -ldl

typedef decltype(dlopen(0, 0)) libtype;

inline auto libload(const std::string & path) -> libtype
{
	return dlopen(path.c_str(), RTLD_LAZY);
}
inline auto libunload(libtype lib)
{
	return dlclose(lib);
}
inline auto libsymbol(libtype lib, const std::string & name) -> decltype(dlsym(0, 0))
{
	return dlsym(lib, name.c_str());
}
inline std::string liberror(libtype)
{
	auto err = dlerror();
	if (!err)
		return std::string();
	return err;
}
inline void libreset(libtype)
{
	dlerror();
}

// Unknown
#else
//#error "No support for loading libraries"
// Minimal implementation to ensure compilation, but error when using it
#define libload(path) ((void*)0)
#define libunload(lib) ((void*)0)
#define libsymbol(lib, name) ((void*)0)
#define liberror(lib) (std::string("No support for dynamic library loading"))
#define liberrorreset(lib) ((void *)0)
#endif

// Get a function from a library and convert it
template<typename T>
std::function<T> libfunctionex(const decltype(lib_impl::lib) lib, const char * func)
{
	return (T *)libsymbol(lib, func);
}
// Get a function and convert it
#define libfunction(lib, func) libfunctionex<decltype(func)>((lib), #func)

// Implementation for a library
// It does not check if a libray is already loaded. It is as default as it can get
class lib_impl
{
public:
	// Load the library
	inline bool load(const std::string & path)
	{
		lib = libload(path);
		return !!lib;
	}
	// Unload the library
	inline void unload()
	{
		libunload(lib);
		lib = decltype(lib)();
	}
	// Get a symbol
	inline auto symbol(const std::string & name) const -> decltype(libsymbol(0, 0))
	{
		return libsymbol(lib, name);
	}
	// Get a function
	template<typename T>
	auto functionex(const std::string & name) const -> decltype(libfunctionex<T>(0, 0))
	{
		return libfunctionex<T>(lib, name);
	}
	// Helper macro for easier fetching a function
#define function(func) functionex<decltype(func)>(std::string(#func))
	// Check if any error have happened
	// Note: Implementation might clear the result after first call
	inline bool error() const
	{
		return !liberror(lib).empty();
	}
	// Get error message
	// Note: Implementation might clear the result after first call
	inline std::string errormsg() const
	{
		return liberror(lib);
	}

	// Check if library is loaded
	inline operator bool() const { return !!lib; }
	// Copy over library to other structure
	inline lib_impl & operator=(const lib_impl & l)
	{
		lib = l.lib;
		return *this;
	}
private:
	decltype(libload(0)) lib = decltype(lib)();
};

#endif // LIBRARY_IMPL_H
