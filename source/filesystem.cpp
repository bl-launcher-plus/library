#include "filesystem.h"

#include <windows.h>

struct FileSearch
{
	HANDLE hFind = INVALID_HANDLE_VALUE;
	WIN32_FIND_DATAA ffd;
};

struct filesystem_impl
{
	FileSearch search;
};

Filesystem::Filesystem()
{
	impl = new filesystem_impl;
	impl->search.ffd.cFileName[0] = 0;
}

Filesystem::~Filesystem()
{
	delete impl;
}

bool Filesystem::load(const std::string & path)
{
	std::string search(path + "/*");
	impl->search.hFind = FindFirstFileA(search.c_str(), &impl->search.ffd);
	if (impl->search.hFind == INVALID_HANDLE_VALUE)
		return false;
	if (impl->search.ffd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
		next();
	return true;
}

void Filesystem::close()
{
	FindClose(impl->search.hFind);
	impl->search.hFind = INVALID_HANDLE_VALUE;
}

bool Filesystem::valid()
{
	if (impl->search.hFind == INVALID_HANDLE_VALUE)
		return false;
	return (impl->search.ffd.cFileName[0] != 0);
}

void Filesystem::next()
{
	while (FindNextFileA(impl->search.hFind, &impl->search.ffd))
	{
		if (impl->search.ffd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
			continue;
		return;
	}

	// Getting here means we are done, or an error occurred
	impl->search.ffd.cFileName[0] = 0;
}


std::string Filesystem::get()
{
	return impl->search.ffd.cFileName;
}

bool Filesystem::createFolder(const std::string & path)
{
	return CreateDirectoryA(path.c_str(), NULL) || GetLastError() == ERROR_ALREADY_EXISTS;
}

bool Filesystem::exists(const std::string & path)
{
	FileSearch search;
	search.hFind = FindFirstFileA(path.c_str(), &search.ffd);
	auto found = search.hFind != INVALID_HANDLE_VALUE;
	if (found)
		FindClose(search.hFind);
	return found;
}
