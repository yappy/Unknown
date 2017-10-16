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

Application::Application(const char *title, int w, int h)
{
	::SDL_Log("Initialize SDL...");
	if (::SDL_Init(SDL_INIT_EVERYTHING)) {
		ThrowLastSDLError<SDLError>();
	}
	m_sdl.reset(this);
	::SDL_Log("Initialize SDL OK");

	::SDL_Log("Create window...");
	SDL_Window *pWindow = ::SDL_CreateWindow(title,
		SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
		w, h,
		0);
	if (pWindow == nullptr) {
		ThrowLastSDLError<SDLError>();
	}
	m_window.reset(pWindow);
	::SDL_Log("Create window OK");
}

Application::~Application()
{}

void Application::Run()
{
	while (1) {
		SDL_Event event;
		while (::SDL_PollEvent(&event)) {
			if (event.type == SDL_QUIT) {
				return;
			}
		}
		// update();
		// render();
		SDL_Delay(16);
	}
}

}
