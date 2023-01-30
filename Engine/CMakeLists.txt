cmake_minimum_required(VERSION 3.0.0)
project(GraphicsEngine VERSION 0.1.0)

add_subdirectory("dependencies/glad")
add_subdirectory("submodules/freetype")
add_subdirectory("submodules/glfw")
add_subdirectory("submodules/spdlog")

file (GLOB_RECURSE LibFiles "src/*.cpp" "src/*.hpp")
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

target_link_libraries(MyGraphicsLib 
	PUBLIC 
		glfw
		glad
		spdlog
	PRIVATE
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