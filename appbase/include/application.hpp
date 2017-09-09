#pragma once

#include <SDL2/SDL.h>
#include <memory>

namespace appbase {

const int DefalutSizeW = 800;
const int DefaultSizeH = 480;


struct SdlDeleter {
	void operator()(void *p) noexcept;
};
struct WindowDeleter {
	void operator()(SDL_Window *p) noexcept;
};
struct RendererDeleter {
	void operator()(SDL_Renderer *p) noexcept;
};

using SdlPtr = std::unique_ptr<void, SdlDeleter>;
using SdlWindowPtr = std::unique_ptr<SDL_Window, WindowDeleter>;
using SdlRendererPtr = std::unique_ptr<SDL_Renderer, RendererDeleter>;


class Application {
public:
	explicit Application(const char *title,
		int w = DefalutSizeW, int h = DefaultSizeH);
	virtual ~Application();

	void Run();
private:
	SdlPtr m_sdl;
	SdlWindowPtr m_window;
	SdlRendererPtr m_renderer;
};

}
