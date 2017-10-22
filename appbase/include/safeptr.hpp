#pragma once

#include <SDL.h>
#include <SDL_image.h>
#include <memory>

namespace appbase {

/* SDL_RWops */
struct RWopsDeleter {
	void operator()(SDL_RWops *p) noexcept
	{
		SDL_RWclose(p);
	}
};
using FilePointer = std::unique_ptr<SDL_RWops, RWopsDeleter>;

/* SDL */
struct SdlDeleter {
	void operator()(void *p) noexcept
	{
		::SDL_Log("Quit SDL");
		::SDL_Quit();
	}
};
using SdlPtr = std::unique_ptr<void, SdlDeleter>;

struct SdlWindowDeleter {
	void operator()(SDL_Window *p) noexcept
	{
		::SDL_Log("Destroy window");
		::SDL_DestroyWindow(p);
	}
};
using SdlWindowPtr = std::unique_ptr<SDL_Window, SdlWindowDeleter>;

/* SDL graphics */
struct SdlRendererDeleter {
	void operator()(SDL_Renderer *p) noexcept
	{
		::SDL_Log("Destroy Renderer");
		::SDL_DestroyRenderer(p);
	}
};
using SdlRendererPtr = std::unique_ptr<SDL_Renderer, SdlRendererDeleter>;

struct SdlSurfaceDeleter {
	void operator()(SDL_Surface *p) noexcept
	{
		::SDL_FreeSurface(p);
	}
};
using SdlSurfacePtr = std::unique_ptr<SDL_Surface, SdlSurfaceDeleter>;

struct SdlTextureDeleter {
	void operator()(SDL_Texture *p) noexcept
	{
		::SDL_DestroyTexture(p);
	}
};
using SdlTexturePtr = std::unique_ptr<SDL_Texture, SdlTextureDeleter>;

/* SDL_image */
struct SdlImageDeleter {
	void operator()(void *p) noexcept
	{
		::SDL_Log("Quit SDL_image");
		::IMG_Quit();
	}
};
using SdlImagePtr = std::unique_ptr<void, SdlImageDeleter>;

}
