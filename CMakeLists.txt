cmake_minimum_required(VERSION 3.7)

# Enable make datail output
set(CMAKE_VERBOSE_MAKEFILE 1)

# Set install dir
set(CMAKE_INSTALL_PREFIX
	"${CMAKE_CURRENT_BINARY_DIR}/dist")

# SDK dir
set(SDK_DIR "${CMAKE_CURRENT_SOURCE_DIR}/sdk")
set(SDK_FONT_DIR "${SDK_DIR}/font")

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
if(WIN32)
	set(SDK_DIR_WIN "${SDK_DIR}/win")
endif()

# SDL2
if(WIN32)
	if(CMAKE_SIZEOF_VOID_P EQUAL 8)
		set(WIN_ARCH "x64")
	else()
		set(WIN_ARCH "x86")
	endif()

	set(SDL2_SDK_DIR "${SDK_DIR_WIN}/SDL2-2.0.7")
	set(SDL2_IMAGE_SDK_DIR "${SDK_DIR_WIN}/SDL2_image-2.0.2")
	set(SDL2_TTF_SDK_DIR "${SDK_DIR_WIN}/SDL2_ttf-2.0.14")

	set(SDL2_INCLUDE_DIRS "${SDL2_SDK_DIR}/include")
	set(SDL2_IMAGE_INCLUDE_DIRS "${SDL2_IMAGE_SDK_DIR}/include")
	set(SDL2_TTF_INCLUDE_DIRS "${SDL2_TTF_SDK_DIR}/include")

	set(SDL2_LIBRARIES
		"${SDL2_SDK_DIR}/lib/${WIN_ARCH}/SDL2.lib"
		"${SDL2_SDK_DIR}/lib/${WIN_ARCH}/SDL2main.lib")
	file(GLOB SDL2_INSTALL_FILES
		"${SDL2_SDK_DIR}/lib/${WIN_ARCH}/*.dll")
	set(SDL2_IMAGE_LIBRARIES
		"${SDL2_IMAGE_SDK_DIR}/lib/${WIN_ARCH}/SDL2_image.lib")
	file(GLOB SDL2_IMAGE_INSTALL_FILES
		"${SDL2_IMAGE_SDK_DIR}/lib/${WIN_ARCH}/*.dll")
	set(SDL2_TTF_LIBRARIES
		"${SDL2_TTF_SDK_DIR}/lib/${WIN_ARCH}/SDL2_ttf.lib")
	file(GLOB SDL2_TTF_INSTALL_FILES
		"${SDL2_TTF_SDK_DIR}/lib/${WIN_ARCH}/*.dll")
	# workaround: Both SDL2_image and SDL2_ttf have zlib1.dll.
	# libpng in SDL2_image causes runtime error if zlib1 in SDL2_ttf is used.
	# Use zlib1 in SDL2_image. (probably newer one)
	list(REMOVE_ITEM SDL2_TTF_INSTALL_FILES
		"${SDL2_TTF_SDK_DIR}/lib/${WIN_ARCH}/zlib1.dll")
else()
	find_package(PkgConfig)
	pkg_check_modules(SDL2 REQUIRED sdl2)
	pkg_check_modules(SDL2_IMAGE REQUIRED SDL2_image)
	pkg_check_modules(SDL2_TTF REQUIRED SDL2_ttf)
	pkg_check_modules(SDL2_MIXER REQUIRED SDL2_mixer)
endif()

# projects
add_subdirectory(appbase)
add_subdirectory(testapp)
