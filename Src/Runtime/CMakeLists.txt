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

    # Rendering
    Rendering/Renderer.h
    Rendering/RenderingResource.h
    Rendering/CommonConstantBuffer.h

    Rendering/RHI/RHI_Device.h
    Rendering/RHI/RHI_SwapChain.h
    Rendering/RHI/RHI_Texture.h
    Rendering/RHI/RHI_Shader.h
    Rendering/RHI/RHI_InputLayout.h
    Rendering/RHI/RHI_VertexBuffer.h
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
    Rendering/RHI/D3D11/D3D11_ConstantBuffer.h
    Rendering/RHI/D3D11/D3D11_CommandList.h
    Rendering/RHI/D3D11/D3D11_PipelineState.h
    Rendering/RHI/D3D11/D3D11_RenderState.h
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

    # Rendering
    Rendering/Renderer.cpp
    Rendering/RenderingResource.cpp
    Rendering/CommonConstantBuffer.cpp

    Rendering/RHI/RHI_Device.cpp
    Rendering/RHI/RHI_SwapChain.cpp
    Rendering/RHI/RHI_Texture.cpp
    Rendering/RHI/RHI_Shader.cpp
    Rendering/RHI/RHI_InputLayout.cpp
    Rendering/RHI/RHI_VertexBuffer.cpp
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
    Rendering/RHI/D3D11/D3D11_ConstantBuffer.h
    Rendering/RHI/D3D11/D3D11_CommandList.cpp
    Rendering/RHI/D3D11/D3D11_PipelineState.cpp
    Rendering/RHI/D3D11/D3D11_RenderState.cpp
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
