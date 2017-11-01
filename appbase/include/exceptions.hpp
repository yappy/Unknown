#pragma once

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <SDL_mixer.h>
#include <stdexcept>
#include <type_traits>

namespace appbase {
namespace error {

class AppBaseError : public std::runtime_error {
public:
	// inheriting constructors
	using runtime_error::runtime_error;
};

class SdlError : public AppBaseError {
public:
	using AppBaseError::AppBaseError;
};

class SdlFileError : public SdlError {
public:
	using SdlError::SdlError;
};

class SdlImageError : public AppBaseError {
public:
	using AppBaseError::AppBaseError;
};

class SdlTtfError : public AppBaseError {
public:
	using AppBaseError::AppBaseError;
};

class SdlMixerError : public AppBaseError {
public:
	using AppBaseError::AppBaseError;
};

template <class T = SdlError>
[[noreturn]] inline void ThrowLastSdlError()
{
	static_assert(std::is_base_of<SdlError, T>::value,
		"T must inherit SdlError");
	throw T(::SDL_GetError());
}

template <class T = SdlImageError>
[[noreturn]] inline void ThrowLastSdlImageError()
{
	static_assert(std::is_base_of<SdlImageError, T>::value,
		"T must inherit SdlImageError");
	throw T(::IMG_GetError());
}

template <class T = SdlTtfError>
[[noreturn]] inline void ThrowLastSdlTtfError()
{
	static_assert(std::is_base_of<SdlTtfError, T>::value,
		"T must inherit SdlTtfError");
	throw T(::TTF_GetError());
}

template <class T = SdlMixerError>
[[noreturn]] inline void ThrowLastSdlMixerError()
{
	static_assert(std::is_base_of<SdlMixerError, T>::value,
		"T must inherit SdlMixerError");
	throw T(::Mix_GetError());
}

}
}
