cmake_minimum_required(VERSION 3.0.0)
project(Graphics VERSION 0.1.0)

set(
	CMAKE_CXX_STANDARD 20
	CMAKE_CXX_STANDARD_REQUIRED YES
	CMAKE_CXX_EXTENSIONS NO
)

set(CMAKE_VERBOSE_MAKEFILE ON)

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
	add_compile_definitions(EG_DEBUG)
endif()

add_subdirectory("Engine")

add_executable(${PROJECT_NAME}  
	"Sandbox/main.cpp"
)

target_link_libraries(${PROJECT_NAME} 
PRIVATE 
	MyGraphicsLib
)