cmake_minimum_required (VERSION 3.8)
project(GraphicsEngine VERSION 0.1.0)

add_subdirectory("dependencies/glad")

file (GLOB_RECURSE LibFiles "*.cpp" "*.hpp")
add_library(MyGraphicsLib STATIC ${LibFiles})

target_compile_definitions(MyGraphicsLib 
PUBLIC 
	$<$<CONFIG:Debug>:EG_DEBUG>
	$<$<CONFIG:Debug>:EG_ENABLE_ASSERTS>
	$<$<CONFIG:Debug>:EG_ENABLE_DEBUG_OUTPUT>
)

target_precompile_headers(MyGraphicsLib
	PRIVATE 
		"pch.hpp"
)
# required vcpkg
find_package(glfw3 CONFIG REQUIRED)
find_package(freetype CONFIG REQUIRED)
find_package(spdlog CONFIG REQUIRED)

target_link_libraries(MyGraphicsLib 
	PUBLIC
		spdlog::spdlog
	PRIVATE
		glad
		glfw
		freetype
)

target_include_directories(MyGraphicsLib
	PUBLIC	
		"${PROJECT_SOURCE_DIR}"
	PRIVATE 
		"dependencies/stb_image"
		glfw
		glad
)