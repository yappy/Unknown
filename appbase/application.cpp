#include <application.hpp>
#include <exceptions.hpp>

using appbase::error::SDLError;
using appbase::error::ThrowLastSDLError;

namespace appbase {

void SdlDeleter::operator()(void *p) noexcept
{
	::SDL_Log("Quit SDL");
	::SDL_Quit();
}

void WindowDeleter::operator()(SDL_Window *p) noexcept
{
	::SDL_Log("Destroy window");
	::SDL_DestroyWindow(p);
}

void RendererDeleter::operator()(SDL_Renderer *p) noexcept
{
	::SDL_Log("Destroy Renderer");
	::SDL_DestroyRenderer(p);
}

Application::Application(const ApplicationSettings &settings) :
	m_settings(settings)
{
	::SDL_Log("Initialize SDL...");
	if (::SDL_Init(SDL_INIT_EVERYTHING)) {
		ThrowLastSDLError<SDLError>();
	}
	m_sdl.reset(this);
	::SDL_Log("Initialize SDL OK");

	::SDL_Log("Disable screen saver...");
	::SDL_DisableScreenSaver();

	::SDL_Log("Create window...");
	SDL_Window *pWindow = ::SDL_CreateWindow(settings.title,
		SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
		settings.w, settings.h,
		0);
	if (pWindow == nullptr) {
		ThrowLastSDLError<SDLError>();
	}
	m_window.reset(pWindow);
	::SDL_Log("Create window OK");
}

void Application::Run()
{
	// count/sec
	Uint64 freq = SDL_GetPerformanceFrequency();
	// count/frame = (count/sec) / (frame/sec)
	Uint64 target = freq / m_settings.fps;

	while (1) {
		Uint64 start = SDL_GetPerformanceCounter();

		SDL_Event event;
		while (::SDL_PollEvent(&event)) {
			if (event.type == SDL_QUIT) {
				return;
			}
		}

		Update();
		Render();

		while (SDL_GetPerformanceCounter() - start < target) {
			SDL_Delay(0);
		}
	}
}

}
