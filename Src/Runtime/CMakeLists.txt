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
    Core/Engine.h
    Core/Application.h
    Core/Subsystem.h
    
    # Math
    Core/Math/Math.h

    Core/Math/Vector2.h
    Core/Math/Vector2I.h
    Core/Math/Vector3.h
    Core/Math/Vector4.h
    Core/Math/Quaternion.h
    Core/Math/Matrix.h

    Core/Math/Rectangle.h
    Core/Math/RectangleI.h
    
    # Platform
    Core/Platform/Platform.h
    Core/Platform/Window.h
    Core/Platform/Windows/Window_Windows.h
)
# cpp files
set(nativeruntime_src
    Core/Engine.cpp
    Core/Application.cpp
    Core/Subsystem.cpp
    # Math
    Core/Math/Math.cpp

    Core/Math/Vector2.cpp
    Core/Math/Vector2I.cpp
    Core/Math/Vector3.cpp
    Core/Math/Vector4.cpp
    Core/Math/Quaternion.cpp
    Core/Math/Matrix.cpp

    Core/Math/Rectangle.cpp
    Core/Math/RectangleI.cpp

    # Platform
    Core/Platform/Platform.cpp
    Core/Platform/Window.cpp
    Core/Platform/Windows/Window_Windows.cpp
)

# project infos
add_library(${PROJECT_NAME} STATIC
    ${nativeruntime_h}
    ${nativeruntime_src}
)
set(files
    ${nativeruntime_h}
    ${nativeruntime_src})

    
# warning levels
# and force source charset utf-8
target_compile_options(
    NativeRuntime PRIVATE
    $<$<CXX_COMPILER_ID:MSVC>:/W4>
    $<$<CXX_COMPILER_ID:MSVC>:/utf-8>
)
# cpp version
target_compile_features(NativeRuntime PRIVATE cxx_std_20)


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

if (ENABLE_TEST)
    add_subdirectory(Test)
endif()
