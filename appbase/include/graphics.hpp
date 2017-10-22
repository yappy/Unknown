#pragma once

#include <SDL.h>
#include "safeptr.hpp"

namespace appbase {
namespace graph {

struct GraphicsSettings {
	bool vsync = false;
	struct {
		uint8_t r = 0;
		uint8_t g = 0;
		uint8_t b = 0;
		uint8_t a = 0xff;
	} clear;
	struct {
		bool jpg = false;
		bool png = true;
		bool tif = false;
	} loadEnable;
};

class GraphicsManager final {
public:
	GraphicsManager(const GraphicsManager &) = delete;
	GraphicsManager & operator=(const GraphicsManager &) = delete;

	GraphicsManager(const GraphicsSettings &settings,
		const SdlWindowPtr &window);
	~GraphicsManager() = default;

	void Clear();
	void Present();

	// Can be called in sub thread
	SdlSurfacePtr LoadImage(const std::string &path);
	// Must be called in main thread
	SdlTexturePtr CreateTexture(const SdlSurfacePtr &surface);
	// All in one
	SdlTexturePtr LoadTexture(const std::string &path)
	{
		return CreateTexture(LoadImage(path));
	}

private:
	GraphicsSettings m_settings;

	SdlRendererPtr m_renderer;
	SdlImagePtr m_sdl_image;
};

}
}
