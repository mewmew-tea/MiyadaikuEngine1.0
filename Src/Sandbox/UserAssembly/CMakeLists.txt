cmake_minimum_required(VERSION 3.22)
enable_language(CSharp)

project(UserAssembly VERSION 0.1.0 LANGUAGES CSharp)

file(GLOB UserAssembly_Files
	Base.cs
	UserAssemblyMain.cs
	UserScript.cs
)


add_library(${PROJECT_NAME} SHARED
    ${UserAssembly_Files}
)

set_target_properties(${PROJECT_NAME} PROPERTIES VS_DOTNET_REFERENCES "System;System.Xml;System.Xml.Linq;System.IO.Compression;Microsoft.CSharp;System.IO.Compression.FileSystem;System.Runtime.CompilerServices.Unsafe.dll")
set_property(TARGET ${PROJECT_NAME} PROPERTY VS_PACKAGE_REFERENCES "Newtonsoft.Json_13.0.1") 
#target_compile_options(EffekseerCoreGUI PRIVATE "/unsafe")
#target_link_libraries(EffekseerCoreGUI PRIVATE EffekseerCore Viewer)
