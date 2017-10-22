#pragma once

#include <SDL.h>
#include <memory>
#include "safeptr.hpp"
#include "exceptions.hpp"
#include "file.hpp"
#include "graphics.hpp"

namespace appbase {

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

	Application(const ApplicationSettings &settings,
		const graph::GraphicsSettings &graphSettings);
	virtual ~Application() = default;

	virtual void Update() = 0;
	virtual void Render() = 0;

	void Run();

	const std::unique_ptr<graph::GraphicsManager> &Graph()
	{
		return m_graph;
	}

private:
	ApplicationSettings m_settings;

	// destruct in reversed order
	SdlPtr m_sdl;
	SdlWindowPtr m_window;
	std::unique_ptr<graph::GraphicsManager> m_graph;
};

}
