# Absolute minimum cmake version
cmake_minimum_required(VERSION 3.1 FATAL_ERROR)

#Include MologieDetours cmake file
include(MologieDetours.cmake)

# The name of the project
set(LIB_NAME BLoader)

# The libraries to load
set(LIBRARIES 
	"MologieDetours"
	"psapi"
	)

# Activate C++14 on MingW cross-compiling situations.
set (CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
# Sources and headers to be included in compilation
set(SOURCES_TORQUE
	"source/torque/functions.cpp"
	"source/torque/scanner.cpp"
	"source/torque/torque.cpp"
	"source/torque/utility.cpp"
	)

set(HEADERS_TORQUE
	"include/torque/functions.h"
	"include/torque/scanner.h"
	"include/torque/torque.h"
	"include/torque/types.h"
	"include/torque/utility.h"
	)

set(SOURCES
	"source/bloader.cpp"
	"source/dllmain.cpp"
	"source/engine.cpp"
	"source/filesystem.cpp"
	"source/library.cpp"
	"${SOURCES_TORQUE}"
	)

set(HEADERS
	"include/blmodule.h"
	"include/bloader.h"
	"include/bloader.hpp"
	"include/engine.h"
	"include/filesystem.h"
	"include/library.h"
	"include/library_impl.h"
	"${HEADERS_TORQUE}"
	"project/blibrary.h"
	)

# Include directories
include_directories("include/")
include_directories("include/torque")
include_directories("project/")


# Make sure functions are exported
add_definitions(-DBLOADER_DLL=1)

# Compile as a library
add_library(${LIB_NAME} SHARED ${SOURCES} ${HEADERS})
# Multi threaded
if(MSVC)
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /MP")
else()
	SET_TARGET_PROPERTIES(BLoader PROPERTIES LINK_FLAGS "-static-libgcc -static-libstdc++")
endif()


# VS filters
source_group("torque" FILES ${SOURCES_TORQUE} ${HEADERS_TORQUE})

# Link the library with more libraries, if needed
target_link_libraries(${LIB_NAME} ${LIBRARIES})
