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
    Core/Object/Object.h
    
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
    
    # Timer
    Core/Timer/Timer.h

    # Rendering
    Rendering/Renderer.h
    Rendering/RenderingResource.h
    Rendering/CommonConstantBuffer.h
    Rendering/Model.h

    Rendering/RHI/RHI_Device.h
    Rendering/RHI/RHI_SwapChain.h
    Rendering/RHI/RHI_Texture.h
    Rendering/RHI/RHI_Shader.h
    Rendering/RHI/RHI_InputLayout.h
    Rendering/RHI/RHI_VertexBuffer.h
    Rendering/RHI/RHI_IndexBuffer.h
    Rendering/RHI/RHI_ConstantBuffer.h
    Rendering/RHI/RHI_CommandList.h
    Rendering/RHI/RHI_PipelineState.h
    Rendering/RHI/RHI_RenderState.h

    # D3D11
    Rendering/RHI/D3D11/D3D11Common.h
    Rendering/RHI/D3D11/D3D11_Device.h
    Rendering/RHI/D3D11/D3D11_SwapChain.h
    Rendering/RHI/D3D11/D3D11_Texture.h
    Rendering/RHI/D3D11/D3D11_Shader.h
    Rendering/RHI/D3D11/D3D11_InputLayout.h
    Rendering/RHI/D3D11/D3D11_VertexBuffer.h
    Rendering/RHI/D3D11/D3D11_IndexBuffer.h
    Rendering/RHI/D3D11/D3D11_ConstantBuffer.h
    Rendering/RHI/D3D11/D3D11_CommandList.h
    Rendering/RHI/D3D11/D3D11_PipelineState.h
    Rendering/RHI/D3D11/D3D11_RenderState.h

    # Scripting
    Scripting/Scripting.h
    Scripting/Component.h
    Scripting/ScriptClassInstance.h
    Scripting/ScriptClassTypeInfo.h
    Scripting/ScriptCommon.h
    Scripting/ScriptFieldInfo.h
    Scripting/ScriptMethod.h
    Scripting/ScriptPropertyInfo.h

    # IPC
    IPC/IPC.h
    IPC/IPCSocket.h
    IPC/Command/IPCCommand.h
    IPC/Command/IPCCommand_SetUpIPC.h
    IPC/Command/IPCCommand_GetComponentTypeInfos.h
    IPC/Command/IPCCommand_GetComponents.h
)
# cpp files
set(nativeruntime_src
    Core/Engine.cpp
    Core/Application.cpp
    Core/Subsystem.cpp
    Core/Object/Object.cpp
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

    # Timer
    Core/Timer/Timer.cpp


    # Rendering
    Rendering/Renderer.cpp
    Rendering/RenderingResource.cpp
    Rendering/CommonConstantBuffer.cpp
    Rendering/Model.cpp

    Rendering/RHI/RHI_Device.cpp
    Rendering/RHI/RHI_SwapChain.cpp
    Rendering/RHI/RHI_Texture.cpp
    Rendering/RHI/RHI_Shader.cpp
    Rendering/RHI/RHI_InputLayout.cpp
    Rendering/RHI/RHI_VertexBuffer.cpp
    Rendering/RHI/RHI_IndexBuffer.cpp
    Rendering/RHI/RHI_ConstantBuffer.cpp
    Rendering/RHI/RHI_CommandList.cpp
    Rendering/RHI/RHI_PipelineState.cpp
    Rendering/RHI/RHI_RenderState.cpp

    # D3D11
    Rendering/RHI/D3D11/D3D11_Device.cpp
    Rendering/RHI/D3D11/D3D11_SwapChain.cpp
    Rendering/RHI/D3D11/D3D11_Texture.cpp
    Rendering/RHI/D3D11/D3D11_Shader.cpp
    Rendering/RHI/D3D11/D3D11_InputLayout.cpp
    Rendering/RHI/D3D11/D3D11_VertexBuffer.cpp
    Rendering/RHI/D3D11/D3D11_IndexBuffer.cpp
    Rendering/RHI/D3D11/D3D11_ConstantBuffer.cpp
    Rendering/RHI/D3D11/D3D11_CommandList.cpp
    Rendering/RHI/D3D11/D3D11_PipelineState.cpp
    Rendering/RHI/D3D11/D3D11_RenderState.cpp

    # Scripting
    Scripting/Scripting.cpp
    Scripting/Component.cpp
    Scripting/ScriptClassInstance.cpp
    Scripting/ScriptClassTypeInfo.cpp
    Scripting/ScriptCommon.cpp
    Scripting/ScriptFieldInfo.cpp
    Scripting/ScriptMethod.cpp
    Scripting/ScriptPropertyInfo.cpp

    # IPC
    IPC/IPC.cpp
    IPC/IPCSocket.cpp
    IPC/Command/IPCCommand.cpp
    IPC/Command/IPCCommand_SetUpIPC.cpp
    IPC/Command/IPCCommand_GetComponentTypeInfos.cpp
    IPC/Command/IPCCommand_GetComponents.cpp
)

# nlohmann json
set(NLOHMANN_JSON_DIR ${CMAKE_CURRENT_SOURCE_DIR}/External/json/single_include)

# dear imgui
set(IMGUI_SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/External/imgui)
set(IMGUI_BACKENDS_DIR ${IMGUI_SOURCE_DIR}/backends)
set(IMGUI_JA_DIR ${CMAKE_CURRENT_SOURCE_DIR}/External/imgui_ja)

set(imgui_src
    ${IMGUI_SOURCE_DIR}/imgui_demo.cpp
    ${IMGUI_SOURCE_DIR}/imgui_draw.cpp
    ${IMGUI_SOURCE_DIR}/imgui_tables.cpp
    ${IMGUI_SOURCE_DIR}/imgui.cpp
    ${IMGUI_SOURCE_DIR}/imgui_widgets.cpp
    ${IMGUI_BACKENDS_DIR}/imgui_impl_win32.cpp
    ${IMGUI_BACKENDS_DIR}/imgui_impl_dx11.cpp
)

# assimp
set(ASSIMP_DIR ${CMAKE_CURRENT_SOURCE_DIR}/External/Assimp)
set(ASSIMP_INCLUDE_DIR ${ASSIMP_DIR}/include)

# project infos
add_library(${PROJECT_NAME} STATIC
    ${nativeruntime_h}
    ${nativeruntime_src}
    ${imgui_src}
)
set(files
    ${nativeruntime_h}
    ${nativeruntime_src})
    
target_include_directories(${PROJECT_NAME} PRIVATE
    ${IMGUI_SOURCE_DIR}
    ${IMGUI_BACKENDS_DIR}
    ${IMGUI_JA_DIR}
    ${NLOHMANN_JSON_DIR}
    ${ASSIMP_INCLUDE_DIR}
)

# link external libs
include_directories(External/mono/Include) 
#link_directories(${CMAKE_CURRENT_SOURCE_DIR}/External/mono/Lib/)
target_link_libraries(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/External/mono/Lib/mono-2.0-sgen.lib)
add_dependencies(${PROJECT_NAME} ExternalProject_assimp)

target_link_directories(${PROJECT_NAME} PUBLIC ${ASSIMP_BUILD_DIR}/src/ExternalProject_assimp-build/lib/Debug)
target_link_libraries(${PROJECT_NAME} PUBLIC 
    debug "${ASSIMP_BUILD_DIR}/src/ExternalProject_assimp-build/lib/Debug/assimp-vc143-mtd.lib" 
    optimized "${ASSIMP_BUILD_DIR}/src/ExternalProject_assimp-build/lib/Release/assimp-vc143-mt.lib")

    
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

add_custom_command(TARGET NativeRuntime PRE_BUILD COMMAND ${CMAKE_COMMAND} -E copy_directory ${ASSIMP_BUILD_DIR}/src/ExternalProject_assimp-build/include/assimp/ ${ASSIMP_INCLUDE_DIR}/assimp/)

# out dirs
set( 
  CMAKE_ARCHIVE_OUTPUT_DIRECTORY_DEBUG libs/Debug
)
set( 
  CMAKE_ARCHIVE_OUTPUT_DIRECTORY_RELEASE libs/Release
)

add_subdirectory(External)

if (ENABLE_TEST)
    add_subdirectory(Test)
endif()
