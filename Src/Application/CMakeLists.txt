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


# source codes
# header files
set(application_h
    Application.h
)
# cpp files
set(application_src
    Application.cpp
    main.cpp
)

add_executable(Application ${application_h} ${application_src})


# link
target_link_libraries(Application NativeRuntime)

# warning levels
target_compile_options(
  Application PRIVATE
   $<$<CXX_COMPILER_ID:MSVC>:/W4>
)
# cpp version
target_compile_features(Application PRIVATE cxx_std_20)

