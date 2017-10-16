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

template <class T> 
[[noreturn]] inline void ThrowLastSDLError()
{
	static_assert(std::is_base_of<SDLError, T>::value,
		"T must inherit SDLError");
	throw T(::SDL_GetError());
}

}
}
