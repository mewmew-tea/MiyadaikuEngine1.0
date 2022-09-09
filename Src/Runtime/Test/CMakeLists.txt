cmake_minimum_required(VERSION 3.22)

project(NativeRuntime_Test CXX)


# test
enable_testing()
if (ENABLE_TEST)
	include(FetchContent)
	FetchContent_Declare(
	  googletest
	  URL https://github.com/google/googletest/archive/5376968f6948923e2411081fd9372e71a59d8e77.zip
	)
	# For Windows: Prevent overriding the parent project's compiler/linker settings
	set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
	FetchContent_MakeAvailable(googletest)
endif()
include(GoogleTest)

set (native_runtime_test_h
	TestEnvironment.h
	TestUtils_Math.h
	../Core/Application.h
)

set(native_runtime_test_src
    main.cpp
	TestEnvironment.cpp
	Application_Test.cpp
	Engine_Test.cpp
	Subsystem_Test.cpp

	Math_Matrix_Test.cpp
	Math_Quaternion_Test.cpp
	
	Math_Vector2_Test.cpp
	Math_Vector2I_Test.cpp
	Math_Vector3_Test.cpp
	Math_Vector4_Test.cpp

	Math_Rectangle_Test.cpp
	Math_RectangleI_Test.cpp
)


add_executable(${PROJECT_NAME} ${native_runtime_test_src} ${native_runtime_test_h})

# warning levels
target_compile_options(
  NativeRuntime_Test PRIVATE
   $<$<CXX_COMPILER_ID:MSVC>:/W4>
)
# cpp version
target_compile_features(NativeRuntime_Test PRIVATE cxx_std_20)

target_link_libraries(${PROJECT_NAME} NativeRuntime GTest::gtest  GTest::gtest_main 
    )

add_compile_definitions(TESTS_ENABLED)

# copy assets
add_custom_command(TARGET NativeRuntime_Test POST_BUILD COMMAND ${CMAKE_COMMAND} -E make_directory ${PROJECT_BINARY_DIR}/Assets/)
add_custom_command(TARGET NativeRuntime_Test POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_SOURCE_DIR}/Assets/ ${PROJECT_BINARY_DIR}/Assets/)
# for CI(GitHub Actions)
add_custom_command(TARGET NativeRuntime_Test POST_BUILD COMMAND ${CMAKE_COMMAND} -E make_directory $<TARGET_FILE_DIR:NativeRuntime_Test>/Assets/)
add_custom_command(TARGET NativeRuntime_Test POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_SOURCE_DIR}/Assets/ $<TARGET_FILE_DIR:NativeRuntime_Test>/Assets/)


gtest_discover_tests(${PROJECT_NAME})
