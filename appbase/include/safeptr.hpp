#pragma once

#include <SDL.h>
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

struct WindowDeleter {
	void operator()(SDL_Window *p) noexcept
	{
		::SDL_Log("Destroy window");
		::SDL_DestroyWindow(p);
	}
};
using SdlWindowPtr = std::unique_ptr<SDL_Window, WindowDeleter>;

/* SDL graphics */
struct RendererDeleter {
	void operator()(SDL_Renderer *p) noexcept
	{
		::SDL_Log("Destroy Renderer");
		::SDL_DestroyRenderer(p);
	}
};
using SdlRendererPtr = std::unique_ptr<SDL_Renderer, RendererDeleter>;

}
