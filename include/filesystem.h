#pragma once
#ifndef FILESYSTEM_H
#define FILESYSTEM_H

#include <string>

/*
 * Will look for all files in a certain path
 */

class Filesystem
{
public:
	Filesystem();
	~Filesystem();
	// Load a path
	bool load(const std::string & path);
	// Close the path
	void close();

	// Check if current file is valid
	bool valid();
	// Get next file
	void next();
	// Get current file name
	std::string get();

	// Create a folder
	static bool createFolder(const std::string & path);

private:
	struct filesystem_impl * impl;
};

#endif // FILESYSTEM_H
