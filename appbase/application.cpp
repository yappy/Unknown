#include <SDL2/SDL.h>

#include <application.hpp>
#include <exceptions.hpp>

Application::Application()
{
	::SDL_Log("Initialize SDL...");
	if (::SDL_Init(SDL_INIT_EVERYTHING)) {
		ThrowLastSDLError<SDLError>();
	}
	::SDL_Log("Initialize SDL OK");
}

Application::~Application()
{
	::SDL_Log("Quit SDL...");
	::SDL_Quit();
	::SDL_Log("OK");
}
