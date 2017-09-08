cmake_minimum_required(VERSION 3.7)
project(basetest)

set(SOURCE_FILES
	testmain.cpp)

add_executable(${PROJECT_NAME} ${SOURCE_FILES})

target_include_directories(${PROJECT_NAME} PRIVATE appbase)
target_link_libraries(${PROJECT_NAME} appbase)
