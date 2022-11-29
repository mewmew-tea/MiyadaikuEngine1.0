cmake_minimum_required(VERSION 3.22)

# effekseer
set(BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(BUILD_DX9 OFF CACHE BOOL "" FORCE)
set(BUILD_DX11 ON CACHE BOOL "" FORCE)
set(BUILD_DX12 OFF CACHE BOOL "" FORCE)
set(BUILD_GL OFF CACHE BOOL "" FORCE)
set(BUILD_VULKAN OFF CACHE BOOL "" FORCE)
add_subdirectory(Effekseer)
set_target_properties(Effekseer PROPERTIES FOLDER ExternalLibraries/Effekseer)
set_target_properties(EffekseerRendererDX11 PROPERTIES FOLDER ExternalLibraries/Effekseer)
set_target_properties(EffekseerSoundDSound PROPERTIES FOLDER ExternalLibraries/Effekseer)
set_target_properties(EffekseerSoundXAudio2 PROPERTIES FOLDER ExternalLibraries/Effekseer)
