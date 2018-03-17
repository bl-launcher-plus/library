# Absolute minimum cmake version
cmake_minimum_required(VERSION 2.8 FATAL_ERROR)

# The name of the project
set(APP_NAME BLoader)

set(LIB_NAME BLoader_lib)

# Sources and headers to be included in compilation
set(SOURCES
	"source/bloader.cpp"
	"source/dllmain.cpp"
	"source/engine.cpp"
	"source/library.cpp"
	"source/Torque.cpp"
	)

set(HEADERS
	"include/blmodule.h"
	"include/bloader.h"
	"include/engine.h"
	"include/library.h"
	"include/library_impl.h"
	"include/Torque.h"
	"project/blibrary.h"
	)

# Include directories
include_directories("include/")
include_directories("project/")

# Compile as a library
add_library(${LIB_NAME} ${SOURCES} ${HEADERS})

# And as a DLL as well.
add_library(${APP_NAME} SHARED ${SOURCES} ${HEADERS})

# Link the library with more libraries, if needed
target_link_libraries(${APP_NAME})
