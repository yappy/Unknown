cmake_minimum_required(VERSION 3.7)
project(testapp)

set(SOURCE_FILES
	testmain.cpp)

set(RESOURCE_DIR "res")

add_executable(${PROJECT_NAME} ${SOURCE_FILES})

# appbase include and link (SDL dependency will be resolved)
target_include_directories(${PROJECT_NAME} PRIVATE appbase)
target_link_libraries(${PROJECT_NAME} PRIVATE appbase)

file(COPY "${RESOURCE_DIR}"
	DESTINATION "${CMAKE_CURRENT_BINARY_DIR}")
file(COPY "${SDK_FONT_DIR}/ipaexg00301"
	DESTINATION "${CMAKE_CURRENT_BINARY_DIR}/${RESOURCE_DIR}")

#install
install(TARGETS ${PROJECT_NAME}
	RUNTIME DESTINATION ${PROJECT_NAME})
install(DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/${RESOURCE_DIR}
	DESTINATION ${PROJECT_NAME})
install(FILES
	${SDL2_INSTALL_FILES}
	${SDL2_IMAGE_INSTALL_FILES}
	${SDL2_TTF_INSTALL_FILES}
	DESTINATION ${PROJECT_NAME})
