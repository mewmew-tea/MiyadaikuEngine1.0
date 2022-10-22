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
    ../Runtime/Core/Application.h
)
# cpp files
set(application_src
    main.cpp
)

add_executable(Application ${application_h} ${application_src})


# link
target_link_libraries(Application NativeRuntime)
target_link_libraries(Application UserAssembly)

# warning levels
target_compile_options(
  Application PRIVATE
   $<$<CXX_COMPILER_ID:MSVC>:/W4>
)
# cpp version
target_compile_features(Application PRIVATE cxx_std_20)

# copy assets
add_custom_command(TARGET Application POST_BUILD COMMAND ${CMAKE_COMMAND} -E make_directory $<TARGET_FILE_DIR:Application>/Assets/)
add_custom_command(TARGET Application POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_SOURCE_DIR}/Assets/ $<TARGET_FILE_DIR:Application>/Assets/)

add_custom_command(TARGET Application POST_BUILD COMMAND ${CMAKE_COMMAND} -E make_directory ${PROJECT_BINARY_DIR}/Assets/)
add_custom_command(TARGET Application POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_SOURCE_DIR}/Assets/ ${PROJECT_BINARY_DIR}/Assets/)

# build CSharp scripts
add_custom_command(TARGET Application PRE_BUILD COMMAND cmd //c ${PROJECT_BINARY_DIR}/Assets/BuildScripts.bat)
add_custom_command(TARGET Application PRE_BUILD COMMAND cmd //c $<TARGET_FILE_DIR:Application>/Assets/BuildScripts.bat)

# copy assemblies
add_custom_command(TARGET Application POST_BUILD COMMAND ${CMAKE_COMMAND} -E make_directory ${PROJECT_BINARY_DIR}/Assets/Assembly/)
add_custom_command(TARGET Application POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy_directory $<TARGET_FILE_DIR:UserAssembly>/ $<TARGET_FILE_DIR:Application>/Assets/Assembly/)

# copy dll
add_custom_command(TARGET Application POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_SOURCE_DIR}/dlls/mono-2.0-sgen.dll ${PROJECT_BINARY_DIR})
add_custom_command(TARGET Application POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_SOURCE_DIR}/dlls/mono-2.0-sgen.dll $<TARGET_FILE_DIR:Application>)
# MonoAssembly
add_custom_command(TARGET Application POST_BUILD COMMAND ${CMAKE_COMMAND} -E make_directory ${PROJECT_BINARY_DIR}/MonoAssembly/)
add_custom_command(TARGET Application POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_SOURCE_DIR}/dlls/MonoAssembly/ ${PROJECT_BINARY_DIR}/MonoAssembly/)

add_custom_command(TARGET Application POST_BUILD COMMAND ${CMAKE_COMMAND} -E make_directory $<TARGET_FILE_DIR:Application>/MonoAssembly/)
add_custom_command(TARGET Application POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_SOURCE_DIR}/dlls/MonoAssembly/ $<TARGET_FILE_DIR:Application>/MonoAssembly/)
