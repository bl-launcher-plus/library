# Absolute minimum cmake version
cmake_minimum_required(VERSION 2.8 FATAL_ERROR)

#Include MologieDetours cmake file
include(MologieDetours.cmake)

# The name of the project
set(LIB_NAME BLoader)

set(LIBRARIES 
	${LIB_NAME}
	"MologieDetours"
	)

# Sources and headers to be included in compilation
set(SOURCES
	"source/bloader.cpp"
	"source/dllmain.cpp"
	"source/engine.cpp"
	"source/filesystem.cpp"
	"source/library.cpp"
	"source/Torque.cpp"
	)

set(HEADERS
	"include/blmodule.h"
	"include/bloader.h"
	"include/bloader.hpp"
	"include/engine.h"
	"include/filesystem.h"
	"include/library.h"
	"include/library_impl.h"
	"include/Torque.h"
	"project/blibrary.h"
	)

# Include directories
include_directories("include/")
include_directories("project/")

# Make sure functions are exported
add_definitions(-DBLOADER_DLL=1)

# Compile as a library
add_library(${LIB_NAME} SHARED ${SOURCES} ${HEADERS})

# Link the library with more libraries, if needed
target_link_libraries(${LIBRARIES})
