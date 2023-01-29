cmake_minimum_required(VERSION 3.0.0)
project(Sandbox VERSION 0.1.0)

set(
	CMAKE_CXX_STANDARD 20
	CMAKE_CXX_STANDARD_REQUIRED YES
	CMAKE_CXX_EXTENSIONS NO
)

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
	add_compile_definitions(EG_DEBUG)
endif()