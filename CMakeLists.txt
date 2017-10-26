cmake_minimum_required(VERSION 3.7)

# Enable make datail output
set(CMAKE_VERBOSE_MAKEFILE 1)

# -cstd=c++14 (cmake cross-platform)
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# GCC specific options
# treat warning as error (main dev env only)
if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    set(CMAKE_CXX_FLAGS "-Wall -Wextra -Wno-unused -Werror -pedantic ${CMAKE_CXX_FLAGS}")
endif()

add_subdirectory(appbase)
add_subdirectory(testapp)
