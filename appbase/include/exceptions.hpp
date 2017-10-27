#pragma once

#include <SDL.h>
#include <SDL_image.h>
#include <stdexcept>
#include <type_traits>

namespace appbase {
namespace error {

class AppBaseError : public std::runtime_error {
public:
	// inheriting constructors
	using runtime_error::runtime_error;
};

class SDLError : public AppBaseError {
public:
	// inheriting constructors
	using AppBaseError::AppBaseError;
};

class SDLFileError : public SDLError {
public:
	// inheriting constructors
	using SDLError::SDLError;
};

class SDLImageError : public AppBaseError {
public:
	// inheriting constructors
	using AppBaseError::AppBaseError;
};

template <class T> 
[[noreturn]] inline void ThrowLastSDLError()
{
	static_assert(std::is_base_of<SDLError, T>::value,
		"T must inherit SDLError");
	throw T(::SDL_GetError());
}

template <class T>
[[noreturn]] inline void ThrowLastSDLImageError()
{
	static_assert(std::is_base_of<SDLImageError, T>::value,
		"T must inherit SDLImageError");
	throw T(::IMG_GetError());
}

}
}
