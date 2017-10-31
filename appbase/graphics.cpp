#include <graphics.hpp>
#include <exceptions.hpp>
#include <array>

namespace appbase {
namespace graph {

using namespace appbase::error;

GraphicsManager::GraphicsManager(const GraphicsSettings &settings,
	const SdlWindowPtr &window) :
	m_settings(settings)
{
	// Create renderer
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
			ThrowLastSdlError<SdlError>();
		}
		::SDL_Log("Create renderer OK");
	}
	// Init SDL_image
	{
		::SDL_Log("Initialize SDL_image...");
		int flags = 0;
		flags |= m_settings.loadEnable.jpg ? IMG_INIT_JPG : 0;
		flags |= m_settings.loadEnable.png ? IMG_INIT_PNG : 0;
		flags |= m_settings.loadEnable.tif ? IMG_INIT_TIF : 0;
		int inited = IMG_Init(flags);
		if ((inited & flags) != flags) {
			throw SdlImageError("SDL_image init failed");
		}
		m_sdl_image.reset(this);
		::SDL_Log("Initialize SDL_image OK");
	}
	// Init SDL_ttf
	{
		::SDL_Log("Initialize SDL_ttf...");
		if (TTF_Init() < 0) {
			ThrowLastSdlTtfError<SdlTtfError>();
		}
		m_sdl_ttf.reset(this);
		::SDL_Log("Initialize SDL_ttf OK");
	}
}

void GraphicsManager::Clear()
{
	if (::SDL_SetRenderDrawColor(m_renderer.get(),
		m_settings.clear.r, m_settings.clear.g, m_settings.clear.b,
		m_settings.clear.a) < 0) {
		ThrowLastSdlError<SdlError>();
	}
	if (::SDL_RenderClear(m_renderer.get()) < 0) {
		ThrowLastSdlError<SdlError>();
	}
}

void GraphicsManager::Present()
{
	::SDL_RenderPresent(m_renderer.get());
}

void GraphicsManager::DrawTexture(const SdlTexturePtr &tex,
	int x, int y)
{
	auto size = GetTextureSize(tex);
	SDL_Rect dst;
	dst.x = x;
	dst.y = y;
	dst.w = std::get<0>(size);
	dst.h = std::get<1>(size);
	::SDL_RenderCopy(m_renderer.get(), tex.get(), nullptr, &dst);
}

SdlSurfacePtr GraphicsManager::LoadImage(const std::string &path)
{
	::SDL_Log("Load image: %s", path.c_str());
	SdlSurfacePtr surface(IMG_Load(path.c_str()));
	if (surface == nullptr) {
		ThrowLastSdlImageError<SdlImageError>();
	}
	return surface;
}

SdlTexturePtr GraphicsManager::CreateTexture(const SdlSurfacePtr &surface)
{
	SdlTexturePtr texture(SDL_CreateTextureFromSurface(
		m_renderer.get(), surface.get()));
	if (texture == nullptr) {
		ThrowLastSdlImageError<SdlImageError>();
	}
	return texture;
}

std::tuple<int, int> GraphicsManager::GetTextureSize(const SdlTexturePtr &tex)
{
	int w, h;
	if(::SDL_QueryTexture(tex.get(), nullptr, nullptr, &w, &h) < 0) {
		ThrowLastSdlError<SdlError>();
	}
	return std::make_tuple(w, h);
}

SdlFontPtr GraphicsManager::LoadFont(const std::string &path, int hsize)
{
	::SDL_Log("Load font: %s", path.c_str());
	SdlFontPtr font(::TTF_OpenFont(path.c_str(), hsize));
	if (font == nullptr) {
		ThrowLastSdlTtfError<SdlTtfError>();
	}
	return font;
}

SdlSurfacePtr GraphicsManager::CreateFontImage(const SdlFontPtr &font,
	char32_t codepoint)
{
	// white
	SDL_Color color = { 255, 255, 255, 255 };
	// codepoint to utf-16 string
	std::array<Uint16, 3> str;
	if (codepoint <= 0xD7FF || (codepoint >= 0xE000 && codepoint <= 0xFFFF)) {
		str[0] = static_cast<Uint16>(codepoint);
		str[1] = 0;
	}
	// surrogate pair seems not to be supported...
	/*
	else if (codepoint >= 0x10000 && codepoint <= 0x10FFFF) {
		auto c = static_cast<uint32_t>(codepoint);
		uint32_t w = ((codepoint & 0x1F00) >> 16) - 1;
		uint32_t x = (codepoint & 0xFF);
		uint32_t xl = (x & 0x3FF);
		uint32_t xh = ((x >> 10) & 0x3F);
		str[0] = 0b1101100000000000 | (w << 6) | xh;
		str[1] = 0b1101110000000000 | xl;
		str[2] = 0;
	}
	*/
	else {
		str[0] = '?';
		str[1] = 0;
	}
	SdlSurfacePtr surface(::TTF_RenderUNICODE_Solid(
		font.get(), str.data(), color));
	if (surface == nullptr) {
		ThrowLastSdlTtfError<SdlTtfError>();
	}
	return surface;
}

}
}
