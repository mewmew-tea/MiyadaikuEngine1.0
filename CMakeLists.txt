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

include(ExternalProject)

# solution
project(MiyadaikuEngine)

set(CMAKE_CONFIGURATION_TYPES "Debug;Release" )

set(ASSIMP_BUILD_DIR ${CMAKE_CURRENT_BINARY_DIR}/External/Build/assimp)

ExternalProject_Add(ExternalProject_assimp
    SOURCE_DIR      ${CMAKE_CURRENT_SOURCE_DIR}/Src/Runtime/External/assimp
    PREFIX          ${CMAKE_CURRENT_BINARY_DIR}/External/Build/assimp
    INSTALL_DIR     ${CMAKE_CURRENT_BINARY_DIR}/External/Install/assimp
    CMAKE_ARGS  -DCMAKE_INSTALL_PREFIX=${EXTERNAL_INSTALL_LOCATION}/assimp
        -DBUILD_SHARED_LIBS=OFF
        -DASSIMP_BUILD_TESTS=OFF
        # -DOpenGL_GL_PREFERENCE=GLVND
        -DASSIMP_BUILD_ASSIMP_TOOLS=OFF
        -DASSIMP_BUILD_ZLIB=ON
        -DASSIMP_NO_EXPORT=ON
        -DASSIMP_BUILD_ALL_IMPORTERS_BY_DEFAULT=OFF
        -DASSIMP_BUILD_glTF_IMPORTER=TRUE
        -DASSIMP_BUILD_glTF2_IMPORTER=TRUE
        -DASSIMP_BUILD_Ogre_IMPORTER=TRUE
)

# add source directory
add_subdirectory(Src)


