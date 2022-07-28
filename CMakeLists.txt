cmake_minimum_required(VERSION 3.22)

# make project filters use folders
set_property(GLOBAL PROPERTY USE_FOLDERS ON)

enable_testing()

# options
option(EDITOR "is editor build" ON)
option(ENABLE_TEST "is build test" ON)

# For MSVC
if (MSVC)
    option(USE_MSVC_RUNTIME_LIBRARY_DLL "Use MSVC runtime library DLL" OFF)
endif()

# solution
if (EDITOR)
	project(MiyadaikuEngine_Editor)
else()
	project(MiyadaikuEngine_Application)
endif()


set(CMAKE_CONFIGURATION_TYPES "Debug;Release" )

# add source directory
add_subdirectory(Src)


