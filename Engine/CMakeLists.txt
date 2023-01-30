cmake_minimum_required(VERSION 3.0.0)
project(GraphicsEngine VERSION 0.1.0)

add_subdirectory("dependencies/glad")

file (GLOB_RECURSE LibFiles "*.cpp" "*.hpp")
add_library(MyGraphicsLib STATIC ${LibFiles})

target_compile_definitions(MyGraphicsLib 
PUBLIC 
	$<$<CONFIG:Debug>:EG_DEBUG>
	$<$<CONFIG:Debug>:EG_ENABLE_ASSERTS>
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
		glfw
		glad
		spdlog::spdlog
		freetype
)

target_include_directories(MyGraphicsLib
	PUBLIC	
		"${PROJECT_SOURCE_DIR}"
		glad
		glfw
	PRIVATE 
		"dependencies/stb_image"
)