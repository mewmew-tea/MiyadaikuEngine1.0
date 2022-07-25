cmake_minimum_required(VERSION 3.22)

project(Application CXX)

if (MSVC)
    if(USE_MSVC_RUNTIME_LIBRARY_DLL)
        cmake_policy(SET CMP0091 NEW)
        set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>DLL")
    else()
        cmake_policy(SET CMP0091 NEW)
        set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")
    endif()
endif()


add_executable(MiyadaikuApplication main.cpp)


# warning levels
target_compile_options(
  NativeRuntime PRIVATE
   $<$<CXX_COMPILER_ID:MSVC>:/W4>
)
# cpp version
target_compile_features(NativeRuntime PRIVATE cxx_std_20)

