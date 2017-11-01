#include <application.hpp>

namespace appbase {

using namespace appbase::error;

Application::Application(const ApplicationSettings &settings,
	const graph::GraphicsSettings &graphSettings) :
	m_settings(settings)
{
	SDL_version compiled, linked;
	SDL_VERSION(&compiled);
	SDL_GetVersion(&linked);
	::SDL_Log("SDL compiled version: %d.%d.%d",
		compiled.major, compiled.minor, compiled.patch);
	::SDL_Log("SDL linked version  : %d.%d.%d",
		linked.major, linked.minor, linked.patch);

	::SDL_Log("Initialize SDL...");
	if (::SDL_Init(SDL_INIT_EVERYTHING) < 0) {
		ThrowLastSdlError<SdlError>();
	}
	m_sdl.reset(this);
	::SDL_Log("Initialize SDL OK");

	// Default is disabled if SDL 2.0.2 or later
	if (::SDL_IsScreenSaverEnabled()) {
		::SDL_Log("Disable screen saver...");
		::SDL_DisableScreenSaver();
	}

	::SDL_Log("Create window...");
	m_window.reset(::SDL_CreateWindow(settings.title,
		SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
		settings.w, settings.h,
		0));
	if (m_window == nullptr) {
		ThrowLastSdlError<SdlError>();
	}
	::SDL_Log("Create window OK");

	m_graph = std::make_unique<graph::GraphicsManager>(
		graphSettings, m_window);
	m_input = std::make_unique<input::InputManager>();
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

		// process frame
		m_input->ProcessFrame();

		// frame update
		Update();
		// render
		m_graph->Clear();
		Render();
		m_graph->Present();

		while (SDL_GetPerformanceCounter() - start < target) {
			SDL_Delay(0);
		}
	}
}

}
