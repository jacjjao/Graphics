cmake_minimum_required (VERSION 3.8)
project(Graphics VERSION 0.1.0)

set(
	CMAKE_CXX_STANDARD 20
	CMAKE_CXX_STANDARD_REQUIRED YES
	CMAKE_CXX_EXTENSIONS NO
	CMAKE_VERBOSE_MAKEFILE ON
)

add_subdirectory("Engine")
add_subdirectory("Sandbox")