cmake_minimum_required(VERSION 3.0.0)
project(Graphics VERSION 0.1.0)

set(
	CMAKE_CXX_STANDARD 20
	CMAKE_CXX_STANDARD_REQUIRED YES
	CMAKE_CXX_EXTENSIONS NO
)

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
	add_compile_definitions(DEBUG=1)
endif()

set(ROOT_DIR "${CMAKE_SOURCE_DIR}")
configure_file("${PROJECT_SOURCE_DIR}/configuration/root_directory.h.in" 
			   "${PROJECT_SOURCE_DIR}/configuration/root_directory.h")

file (GLOB_RECURSE LibFiles "src/*.cpp")

add_library(glad "dependencies/glad/src/glad.c")

add_library(MyGraphicsLib 
STATIC 
	${LibFiles}
)

add_subdirectory("submodules/freetype")
add_subdirectory("submodules/glfw")

add_executable(${PROJECT_NAME}  
	"demo/main.cpp"
)

target_precompile_headers(${PROJECT_NAME} 
PRIVATE
	"include/pch.hpp"
)

target_link_libraries(${PROJECT_NAME} 
PUBLIC 
	MyGraphicsLib
	glfw
	glad
	freetype
)

target_include_directories(MyGraphicsLib
PRIVATE
	glfw
	"dependencies/glad/include"
	"dependencies/stb_image"
	"submodules/freetype/include"
)

target_include_directories(${PROJECT_NAME}
PRIVATE
	"dependencies/glad/include"
)