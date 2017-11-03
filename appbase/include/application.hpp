#pragma once

#include <SDL.h>
#include <memory>
#include "safeptr.hpp"
#include "exceptions.hpp"
#include "file.hpp"
#include "graphics.hpp"
#include "sound.hpp"
#include "input.hpp"

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
		const graph::GraphicsSettings &graph_settings,
		const sound::SoundSettings &sound_settings);
	virtual ~Application() = default;

	virtual void Update() = 0;
	virtual void Render() = 0;

	void Run();

	graph::GraphicsManager &Graph()
	{
		return *m_graph;
	}
	sound::SoundManager &Sound()
	{
		return *m_sound;
	}
	input::InputManager &Input()
	{
		return *m_input;
	}

private:
	ApplicationSettings m_settings;

	// destruct in reversed order
	SdlPtr m_sdl;
	SdlWindowPtr m_window;
	std::unique_ptr<graph::GraphicsManager> m_graph;
	std::unique_ptr<sound::SoundManager> m_sound;
	std::unique_ptr<input::InputManager> m_input;
};

}
