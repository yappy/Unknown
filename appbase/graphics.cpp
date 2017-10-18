#include <graphics.hpp>
#include <exceptions.hpp>

namespace appbase {
namespace graph {

using appbase::error::SDLError;
using appbase::error::ThrowLastSDLError;

GraphicsManager::GraphicsManager(const GraphicsSettings &settings,
	const SdlWindowPtr &window) :
	m_settings(settings)
{
	Uint32 flags = 0;
	if (m_settings.vsync) {
		flags |= SDL_RENDERER_PRESENTVSYNC;
	}
	::SDL_Log("Create HW renderer...");
	m_renderer.reset(::SDL_CreateRenderer(
		window.get(), -1, flags | SDL_RENDERER_ACCELERATED));
	if (m_renderer == nullptr) {
		::SDL_Log("Create SW renderer...");
		m_renderer.reset(::SDL_CreateRenderer(
			window.get(), -1, flags | SDL_RENDERER_SOFTWARE));
	}
	if (m_renderer == nullptr) {
		ThrowLastSDLError<SDLError>();
	}
	::SDL_Log("Create renderer OK");
}

void GraphicsManager::Clear()
{
	if (::SDL_SetRenderDrawColor(m_renderer.get(),
		m_settings.clear.r, m_settings.clear.g, m_settings.clear.b,
		m_settings.clear.a) < 0) {
		ThrowLastSDLError<SDLError>();
	}
	if (::SDL_RenderClear(m_renderer.get()) < 0) {
		ThrowLastSDLError<SDLError>();
	}
}

void GraphicsManager::Present()
{
	::SDL_RenderPresent(m_renderer.get());
}

}
}
