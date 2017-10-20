cmake_minimum_required(VERSION 3.7)
project(appbase)

set(SOURCE_FILES
	application.cpp
	graphics.cpp
	file.cpp)

# static lib target
add_library(${PROJECT_NAME} STATIC ${SOURCE_FILES})

# include dir for this library
target_include_directories(${PROJECT_NAME} PUBLIC include/)

find_package(PkgConfig)

# find packages
pkg_check_modules(SDL2 REQUIRED sdl2)
pkg_check_modules(SDL2_IMAGE REQUIRED SDL2_image)

# include and link (PUBLIC for other targets which use this target)
target_include_directories(${PROJECT_NAME} PUBLIC
	${SDL2_INCLUDE_DIRS}
	${SDL2_IMAGE_INCLUDE_DIRS})
target_link_libraries(${PROJECT_NAME} PUBLIC
	${SDL2_LIBRARIES}
	${SDL2_IMAGE_LIBRARIES})
