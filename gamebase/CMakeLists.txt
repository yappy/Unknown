cmake_minimum_required(VERSION 3.7)
project(libgamebase)

set(SOURCE_FILES
	application.cpp)

add_executable(${PROJECT_NAME} ${SOURCE_FILES})

find_package(SDL2 REQUIRED)
include_directories(${SDL2_INCLUDE_DIRS})
target_link_libraries(${PROJECT_NAME} ${SDL2_LIBRARIES})
