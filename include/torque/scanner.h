#pragma once
#ifndef SCANNER_H
#define SCANNER_H

#include <memory>
#include <string>
#include <functional>

class Scanner
{
public:
	Scanner();
	Scanner(const std::string & file);

	template<typename T>
	std::function<T> locateFunctionT(const char * pattern, const char * mask)
	{
		return static_cast<T *>(findFunction(pattern, mask));
	}
	template<typename T>
	T locateFunction(const char * pattern, const char * mask)
	{
		(T)(findFunction(pattern, mask));
	}

private:
	std::shared_ptr<struct scanner_impl> impl;

	void init(const char * file);

	void * findFunction(const char * pattern, const char * mask);
};

#endif // SCANNER_H
