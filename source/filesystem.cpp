#include "filesystem.h"

#include <windows.h>

struct filesystem_impl
{
	HANDLE hFind = INVALID_HANDLE_VALUE;
	WIN32_FIND_DATA ffd;
};

Filesystem::Filesystem()
{
	impl = new filesystem_impl;
	impl->ffd.cFileName[0] = 0;
}

Filesystem::~Filesystem()
{
	delete impl;
}

bool Filesystem::load(const std::string & path)
{
	std::string search(path + "/*");
	impl->hFind = FindFirstFileA(search.c_str(), &impl->ffd);
	if (impl->hFind == INVALID_HANDLE_VALUE)
		return false;
	if (impl->ffd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
		next();
	return true;
}
void Filesystem::close()
{
	FindClose(impl->hFind);
	impl->hFind = INVALID_HANDLE_VALUE;
}

bool Filesystem::valid()
{
	if (impl->hFind != INVALID_HANDLE_VALUE)
		return false;
	return impl->ffd.cFileName[0] != 0;
}

void Filesystem::next()
{
	while (FindNextFileA(impl->hFind, &impl->ffd))
	{
		if (impl->ffd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
			continue;
		return;
	}

	// Getting here means we are done, or an error occurred
	impl->ffd.cFileName[0] = 0;
}

std::string Filesystem::get()
{
	return impl->ffd.cFileName;
}

bool Filesystem::createFolder(const std::string & path)
{
	return CreateDirectoryA(path.c_str(), NULL) || GetLastError() == ERROR_ALREADY_EXISTS;
}
