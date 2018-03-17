#include "library.h"
#include "library_impl.h"

/*
 * Library implementation
 */

Library::Library()
{
	// As the destructor will unload the library, keep reference
	// count so it is only unloaded when there is no reference left.
	impl = std::make_shared<lib_impl>();
}

// Load the library
bool Library::load(const std::string & path)
{
	return impl->load(path);
}
// Unload the library
void Library::unload()
{
	impl->unload();
}
// Get a symbol
void * Library::symbol(const std::string & name) const
{
	return (void *)impl->symbol(name);
}
// Check if any error have happened
// Note: Implementation might clear the result after first call
bool Library::error() const
{
	return impl->error();
}
// Get error message
// Note: Implementation might clear the result after first call
std::string Library::errormsg() const
{
	return impl->errormsg();
}

// Check if library is loaded
Library::operator bool() const
{
	return !!impl && *impl;
}
// Copy over library to other structure
Library & Library::operator=(const Library & l)
{
	impl = l.impl;
	return *this;
}
