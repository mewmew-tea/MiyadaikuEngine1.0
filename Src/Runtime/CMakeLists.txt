cmake_minimum_required(VERSION 3.22)

project(NativeRuntime CXX)

if (MSVC)
    if(USE_MSVC_RUNTIME_LIBRARY_DLL)
        cmake_policy(SET CMP0091 NEW)
        set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>DLL")
    else()
        cmake_policy(SET CMP0091 NEW)
        set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")
    endif()
endif()

# source codes
# header files
file(GLOB nativeruntime_h
    NativeRuntime.h
)
# cpp files
set(nativeruntime_src
    NativeRuntimeMain.cpp
    NativeRuntime.cpp
)

# project infos
add_library(${PROJECT_NAME} STATIC
    ${nativeruntime_h}
    ${nativeruntime_src}
)
set(files
    ${nativeruntime_h}
    ${nativeruntime_src})

# generate filters by folders
set(PrefixDir "\\")
set(SrcDir ${CMAKE_CURRENT_LIST_DIR})
source_group(TREE ${SrcDir} PREFIX ${PrefixDir} FILES ${nativeruntime_h})
source_group(TREE ${SrcDir} PREFIX ${PrefixDir} FILES ${nativeruntime_src})

# out dirs
set( 
  CMAKE_ARCHIVE_OUTPUT_DIRECTORY_DEBUG libs/Debug
)
set( 
  CMAKE_ARCHIVE_OUTPUT_DIRECTORY_RELEASE libs/Release
)
