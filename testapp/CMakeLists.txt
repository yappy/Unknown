cmake_minimum_required(VERSION 3.7)
project(testapp)

set(SOURCE_FILES
	testmain.cpp)

set(RESOURCE_DIRS
	res)

add_executable(${PROJECT_NAME} ${SOURCE_FILES})

# appbase include and link (SDL dependency will be resolved)
target_include_directories(${PROJECT_NAME} PRIVATE appbase)
target_link_libraries(${PROJECT_NAME} PRIVATE appbase)

#install
install(TARGETS ${PROJECT_NAME}
	RUNTIME DESTINATION ${PROJECT_NAME})
install(DIRECTORY ${RESOURCE_DIRS}
	DESTINATION ${PROJECT_NAME})
install(FILES
	${SDL2_INSTALL_FILES}
	${SDL2_IMAGE_INSTALL_FILES}
	DESTINATION ${PROJECT_NAME})
