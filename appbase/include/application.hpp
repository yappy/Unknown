#pragma once

#include <SDL.h>
#include <memory>

#include "exceptions.hpp"
#include "file.hpp"

namespace appbase {

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


const int DefalutSizeW = 800;
const int DefaultSizeH = 480;
const int DefaultFps = 60;

struct ApplicationSettings {
	const char *title = "Window title";
	int w = DefalutSizeW;
	int h = DefaultSizeH;
	int fps = DefaultFps;
};

class Application {
public:
	Application(const Application &) = delete;
	Application & operator=(const Application &) = delete;

	explicit Application(const ApplicationSettings &settings);
	virtual ~Application() = default;

	virtual void Update() = 0;
	virtual void Render() = 0;

	void Run();

private:
	ApplicationSettings m_settings;

	SdlPtr m_sdl;
	SdlWindowPtr m_window;
	SdlRendererPtr m_renderer;
};

}
