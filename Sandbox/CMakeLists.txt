cmake_minimum_required (VERSION 3.8)
project(Sandbox VERSION 0.1.0)

add_executable(${PROJECT_NAME}  
	"main.cpp"
)

target_link_libraries(${PROJECT_NAME} 
PRIVATE 
	MyGraphicsLib
)

add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD 
               COMMAND ${CMAKE_COMMAND} -E copy_if_different
               "${CMAKE_BINARY_DIR}/Engine/MyGraphicsLib.dll"              
               "${CMAKE_BINARY_DIR}/Sandbox")

#add_executable("PhysicsDemo"
#	"PhysicsDemo.cpp"
#)

#target_link_libraries("PhysicsDemo"
#PRIVATE 
#	MyGraphicsLib
#)
