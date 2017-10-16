cmake_minimum_required(VERSION 3.7)
project(basetest)

set(SOURCE_FILES
	testmain.cpp)

set(RESOURCE_DIRS
	res)

add_executable(${PROJECT_NAME} ${SOURCE_FILES})

target_include_directories(${PROJECT_NAME} PRIVATE appbase)
target_link_libraries(${PROJECT_NAME} appbase)

file(COPY ${RESOURCE_DIRS}
	DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
