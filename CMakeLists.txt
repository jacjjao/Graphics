cmake_minimum_required(VERSION 3.0.0)
project(Graphics VERSION 0.1.0)

set(
	CMAKE_CXX_STANDARD 20
	CMAKE_CXX_STANDARD_REQUIRED YES
	CMAKE_CXX_EXTENSIONS NO
)

add_executable(${PROJECT_NAME}  
	"src/glCheck.cpp"
	"src/ShaderProgram.cpp"
	"src/VertexArray.cpp"
	"src/VertexBuffer.cpp"
	"src/Rectangle2D.cpp"
	"src/Clock.cpp"
	"src/ElementBuffer.cpp"
	
	"src/FileSystem.cpp"
	"src/Circle2D.cpp"
	"src/Shape.cpp"
	"src/Transformable.cpp"
	"src/Texture.cpp"
	"src/Camera.cpp"
	"src/Line.cpp"
	"src/Color.cpp"
	"src/TextRenderer.cpp"
	"src/Math.cpp"
	"demo/main.cpp"
	#"demo/mario/main.cpp"
)

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
	add_compile_definitions(DEBUG=1)
else()
	add_compile_definitions(DEBUG=0)
endif()

set(ROOT_DIR "${CMAKE_SOURCE_DIR}")
configure_file("${PROJECT_SOURCE_DIR}/configuration/root_directory.h.in" 
			   "${PROJECT_SOURCE_DIR}/configuration/root_directory.h")

add_library(glad "dependencies/glad/src/glad.c")

find_package(glfw3 CONFIG REQUIRED)

target_precompile_headers(${PROJECT_NAME} 
	PRIVATE
		"include/pch.hpp"
)

target_link_libraries(${PROJECT_NAME} 
	PRIVATE
		glfw
		glad
		${CMAKE_SOURCE_DIR}/dependencies/freetype/freetype.lib
)

target_include_directories(${PROJECT_NAME}
	PRIVATE
		"dependencies/glad/include"
		"dependencies/stb_image"
		"dependencies/freetype/include"
)