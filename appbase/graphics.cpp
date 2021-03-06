#include <graphics.hpp>
#include <exceptions.hpp>
#include <array>

namespace appbase {
namespace graph {

using namespace appbase::error;

namespace {
	const uint8_t UnknownChar = '?';
}

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
			ThrowLastSdlError();
		}
		::SDL_Log("Create renderer OK");
	}
	// Init SDL_image
	{
		::SDL_Log("Initialize SDL_image...");
		int flags = 0;
		flags |= m_settings.load_enable.jpg ? IMG_INIT_JPG : 0;
		flags |= m_settings.load_enable.png ? IMG_INIT_PNG : 0;
		flags |= m_settings.load_enable.tif ? IMG_INIT_TIF : 0;
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
			ThrowLastSdlTtfError();
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
		ThrowLastSdlError();
	}
	if (::SDL_RenderClear(m_renderer.get()) < 0) {
		ThrowLastSdlError();
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

void GraphicsManager::DrawStringUtf8(
	const CharTextureMap &ctex, const char *str, int x, int y)
{
	auto p = static_cast<const uint8_t *>(static_cast<const void *>(str));
	while (*p != '\0') {
		uint32_t c = 0;
		if ((*p & 0b1'0000000) == 0b0'0000000) {
			c = *p++;
		}
		else if ((*p & 0b111'00000) == 0b110'11111) {
			uint8_t c1 = *p++;
			uint8_t c2 = *p++;
			if ((c2 & 0b11'000000) != 0b10'000000) {
				break;
			}
			c = ((c1 & 0b000'11111) << 6) | (c2 & 0b00'111111);
			if (c < 0x80) {
				break;
			}
		}
		else if ((*p & 0b1111'0000) == 0b1110'0000) {
			uint8_t c1 = *p++;
			uint8_t c2 = *p++;
			if ((c2 & 0b11'000000) != 0b10'000000) {
				break;
			}
			uint8_t c3 = *p++;
			if ((c3 & 0b11'000000) != 0b10'000000) {
				break;
			}
			c = ((c1 & 0b0000'1111) << 12) |
				((c2 & 0b000'11111) << 6) |
				(c3 & 0b00'111111);
			if (c < 0x800) {
				break;
			}
		}
		else {
			// >16bit codepoint seems not to be supported...
			break;
		}
		// try to find c, try to find UnknownChar, give up
		auto it = ctex.find(c);
		if (it == ctex.end()) {
			it = ctex.find(UnknownChar);
		}
		if (it != ctex.end()) {
			const auto &tex = it->second;
			DrawTexture(tex, x, y);
			x += GetTextureSize(tex).first;
		}
	}
}

SdlSurfacePtr GraphicsManager::LoadImage(const std::string &path)
{
	::SDL_Log("Load image: %s", path.c_str());
	SdlSurfacePtr surface(IMG_Load(path.c_str()));
	if (surface == nullptr) {
		ThrowLastSdlImageError();
	}
	return surface;
}

SdlTexturePtr GraphicsManager::CreateTexture(const SdlSurfacePtr &surface)
{
	SdlTexturePtr texture(SDL_CreateTextureFromSurface(
		m_renderer.get(), surface.get()));
	if (texture == nullptr) {
		ThrowLastSdlImageError();
	}
	return texture;
}

std::pair<int, int> GraphicsManager::GetTextureSize(const SdlTexturePtr &tex)
{
	int w, h;
	if(::SDL_QueryTexture(tex.get(), nullptr, nullptr, &w, &h) < 0) {
		ThrowLastSdlError();
	}
	return std::make_pair(w, h);
}

SdlFontPtr GraphicsManager::LoadFont(const std::string &path, int hsize)
{
	::SDL_Log("Load font: %s", path.c_str());
	SdlFontPtr font(::TTF_OpenFont(path.c_str(), hsize));
	if (font == nullptr) {
		ThrowLastSdlTtfError();
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
		str[0] = UnknownChar;
		str[1] = 0;
	}
	SdlSurfacePtr surface(::TTF_RenderUNICODE_Solid(
		font.get(), str.data(), color));
	if (surface == nullptr) {
		ThrowLastSdlTtfError();
	}
	return surface;
}

CharTextureMap GraphicsManager::CreateFontTextureMap(const SdlFontPtr &font,
	std::initializer_list<std::pair<char32_t, char32_t>> ranges)
{
	CharTextureMap result;
	// Add unknown character
	{
		SdlTexturePtr tex = CreateTexture(
			CreateFontImage(font, UnknownChar));
		result.emplace(UnknownChar, std::move(tex));
	}
	for (const auto &range : ranges) {
		char32_t start = range.first;
		char32_t end = range.second;
		if (start > end) {
			throw std::invalid_argument("start > end");
		}
		for (char32_t c = start; c <= end; c++) {
			SdlTexturePtr tex = CreateTexture(
				CreateFontImage(font, c));
			result.emplace(c, std::move(tex));
		}
	}
	return result;
}

}
}
