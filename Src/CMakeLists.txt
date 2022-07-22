cmake_minimum_required(VERSION 3.22)

# subdirs
add_subdirectory(Runtime)

if (EDITOR)
	add_subdirectory(Editor)
else()
	add_subdirectory(Application)
endif()

