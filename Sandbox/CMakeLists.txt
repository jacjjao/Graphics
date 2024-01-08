cmake_minimum_required(VERSION 3.0.0)
project(Sandbox VERSION 0.1.0)

add_executable(${PROJECT_NAME}  
	"main.cpp"
	#"PhysicsDemo.cpp"
)

target_link_libraries(${PROJECT_NAME} 
PRIVATE 
	MyGraphicsLib
)