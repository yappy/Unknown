cmake_minimum_required(VERSION 3.7)
project(appbase)

set(SOURCE_FILES
	application.cpp
	file.cpp
	graphics.cpp
	input.cpp
	sound.cpp)

# static lib target
add_library(${PROJECT_NAME} STATIC ${SOURCE_FILES})

# include dir for this library
target_include_directories(${PROJECT_NAME} PUBLIC include/)

# include and link (PUBLIC for other targets which use this target)
target_include_directories(${PROJECT_NAME} PUBLIC
	${SDL2_INCLUDE_DIRS}
	${SDL2_IMAGE_INCLUDE_DIRS}
	${SDL2_TTF_INCLUDE_DIRS}
	${SDL2_MIXER_INCLUDE_DIRS})
target_link_libraries(${PROJECT_NAME} PUBLIC
	${SDL2_LIBRARIES}
	${SDL2_IMAGE_LIBRARIES}
	${SDL2_TTF_LIBRARIES}
	${SDL2_MIXER_LIBRARIES})
