cmake_minimum_required(VERSION 3.0.0)
project(Test VERSION 0.1.0)

set(CMAKE_CXX_STANDARD 20)

add_executable(Test main.cpp VectorTests.cpp)

find_package(GTest CONFIG REQUIRED)
target_link_libraries(${PROJECT_NAME} 
	PRIVATE
		GTest::gmock 
		GTest::gtest 
		GTest::gmock_main 
		GTest::gtest_main
)