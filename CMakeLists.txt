cmake_minimum_required(VERSION 3.0.0)
project(Graphics VERSION 0.1.0)

set(
	CMAKE_CXX_STANDARD 20
	CMAKE_CXX_STANDARD_REQUIRED YES
	CMAKE_CXX_EXTENSIONS NO
)

add_executable(${PROJECT_NAME}  
	#headers
	"include/glCheck.hpp"
	"include/ShaderProgram.hpp"
	"include/Vertex.hpp"
	"include/VertexArray.hpp"
	"include/Rectangle2D.hpp"
	"include/VertexBuffer.hpp"
	"include/Utility.hpp"
	"include/Clock.hpp"
	"include/ElementBuffer.hpp"
	"include/FileSystem.hpp"
	"include/Circle2D.hpp"
	"include/Shape.hpp"
	"include/Transformable.hpp"

	#sources
	"src/glCheck.cpp"
	"src/ShaderProgram.cpp"
	"src/VertexArray.cpp"
	"src/VertexBuffer.cpp"
	"src/Vertex.cpp"
	"src/Rectangle2D.cpp"
	"src/Clock.cpp"
	"src/ElementBuffer.cpp"
	"src/Utility.cpp"
	"src/FileSystem.cpp"
	"src/Circle2D.cpp"
	"src/Shape.cpp"
	"src/Transformable.cpp"
	"demo/main.cpp"
)

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
	add_compile_definitions(DEBUG=1)
else()
	add_compile_definitions(DEBUG=0)
endif()

set(ROOT_DIR "${CMAKE_SOURCE_DIR}")
configure_file("${PROJECT_SOURCE_DIR}/configuration/root_directory.h.in" 
			   "${PROJECT_SOURCE_DIR}/configuration/root_directory.h")

find_package(glfw3 CONFIG REQUIRED)
find_package(glad CONFIG REQUIRED)
find_package(glm CONFIG REQUIRED)

target_link_libraries(${PROJECT_NAME} 
	PRIVATE
		glfw
		glad::glad
		glm::glm
)