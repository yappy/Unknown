cmake_minimum_required(VERSION 3.7)

set(CMAKE_VERBOSE_MAKEFILE 1)

set(CMAKE_CXX_FLAGS "-std=c++14 -Wall -Wextra -pedantic")

add_subdirectory(appbase)
add_subdirectory(basetest)
