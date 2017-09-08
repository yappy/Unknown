#pragma once

#include <stdexcept>
#include <type_traits>

class SDLError : public std::runtime_error {
public:
	using runtime_error::runtime_error;
};

template <class T> 
[[noreturn]] inline void ThrowLastSDLError()
{
	static_assert(std::is_base_of<SDLError, T>::value,
		"T must inherit SDLError");
	throw T(SDL_GetError());
}
