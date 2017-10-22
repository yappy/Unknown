#pragma once

#include <stdexcept>
#include <type_traits>

namespace appbase {
namespace error {

class SDLError : public std::runtime_error {
public:
	// inheriting constructors
	using runtime_error::runtime_error;
};

class SDLFileError : public SDLError {
public:
	// inheriting constructors
	using SDLError::SDLError;
};

class SDLImageError : public std::runtime_error {
public:
	// inheriting constructors
	using runtime_error::runtime_error;
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
