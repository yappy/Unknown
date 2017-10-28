cmake_minimum_required(VERSION 3.7)

# Enable make datail output
set(CMAKE_VERBOSE_MAKEFILE 1)

# Set install dir
set(CMAKE_INSTALL_PREFIX "${CMAKE_CURRENT_BINARY_DIR}/dist")


# -cstd=c++14 (cmake cross-platform)
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# GCC specific options
# treat warning as error (main dev env only)
if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
	set(CMAKE_CXX_FLAGS "-Wall -Wextra -Wno-unused -Werror -pedantic ${CMAKE_CXX_FLAGS}")
endif()


# external dependencies
set(SDK_DIR "${CMAKE_CURRENT_SOURCE_DIR}/sdk")
if(WIN32)
	set(SDK_DIR_WIN "${SDK_DIR}/win")
endif()

# SDL2
if(WIN32)
	set(SDL2_SDK_DIR "${SDK_DIR_WIN}/SDL2-2.0.7")
	set(SDL2_IMAGE_SDK_DIR "${SDK_DIR_WIN}/SDL2_image-2.0.2")

	set(SDL2_INCLUDE_DIRS "${SDL2_SDK_DIR}/include")
	set(SDL2_IMAGE_INCLUDE_DIRS "${SDL2_IMAGE_SDK_DIR}/include")

	if(CMAKE_SIZEOF_VOID_P EQUAL 8)
		set(SDL2_LIBRARIES
			"${SDL2_SDK_DIR}/lib/x64/SDL2.lib"
			"${SDL2_SDK_DIR}/lib/x64/SDL2main.lib")
		set(SDL2_IMAGE_LIBRARIES "${SDL2_IMAGE_SDK_DIR}/lib/x64/SDL2_image.lib")
	else()
		set(SDL2_LIBRARIES
			"${SDL2_SDK_DIR}/lib/x86/SDL2.lib"
			"${SDL2_SDK_DIR}/lib/x86/SDL2main.lib")
		set(SDL2_IMAGE_LIBRARIES "${SDL2_IMAGE_SDK_DIR}/lib/x86/SDL2_image.lib")
	endif()
else()
	find_package(PkgConfig)
	pkg_check_modules(SDL2 REQUIRED sdl2)
	pkg_check_modules(SDL2_IMAGE REQUIRED SDL2_image)
endif()

# projects
add_subdirectory(appbase)
add_subdirectory(testapp)
