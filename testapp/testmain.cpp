#include <application.hpp>
#include <array>
#include <string>
#include <vector>

class MyApp : public appbase::Application {
private:
	appbase::SdlTexturePtr m_tex;
	appbase::graph::CharTextureMap m_fonttex;
	appbase::SdlChunkSharedPtr m_se;
public:
	// inheriting constructor
	using Application::Application;

	void Load()
	{
		using appbase::file::FromBasePath;

		m_tex = Graph().LoadTexture(
			FromBasePath("res/sample_8_index.png"));
		auto font = Graph().LoadFont(
			FromBasePath("res/ipaexg00301/ipaexg.ttf"), 32);
		m_fonttex = Graph().CreateFontTextureMap(font,
			{
				// hiragana
				{ 0x3040, 0x309F },
				// katakana
				{ 0x30A0, 0x30FF },
			});

		m_se = Sound().LoadSe(FromBasePath("res/se/tm2_hit005.wav"));
	}

	void Update() override
	{
		const auto &keys = Input().GetKeyState();
		for (int i = 0; i < appbase::input::KeyCount; i++) {
			if (keys[i]) {
				SDL_Log("Key: %s", appbase::input::GetKeyName(i));
			}
		}

		const auto &mouse = Input().GetMouseState();
		for (int i = 0; i < 3; i++) {
			if (mouse.barray[i]) {
				SDL_Log("Mouse: %d (%d, %d)", i, mouse.x, mouse.y);
			}
		}

		if (keys[SDL_SCANCODE_Z]) {
			bool ok = Sound().PlaySe(m_se);
			if (!ok) {
				SDL_Log("Sound effect mixer full");
			}
		}
	}
	void Render() override
	{
		Graph().DrawTexture(m_tex, 0, 0);
		Graph().DrawStringUtf8(m_fonttex, u8"ほわいと", 0, 100);
		Graph().DrawStringUtf8(m_fonttex, u8"ぜっときーでこうかおん", 0, 200);
	}
};

// may be replaced with SDL_main
int main(int argc, char *argv[])
{
	try {
		{
			using namespace appbase::file;
			auto fp = OpenR(FromBasePath("res/test.txt"));
			std::string strdata = appbase::file::ReadAllString(fp);
			SDL_Log(strdata.c_str());
		}
		{
			using namespace appbase::file;
			auto fp = OpenR(FromBasePath("res/multiline.txt"));
			for (const auto &line : appbase::file::ReadAllLines(fp)) {
				SDL_Log(line.c_str());
			}
		}
		{
			using namespace appbase::file;
			ConfigFile config({
				{"system.run", true},
				{"system.title", "Test App"},
				{"graph.clear.r", 0},
				{"graph.clear.g", 0},
				{"graph.clear.b", 0},
			});
			config.Load(FromBasePath("config.txt"));
		}

		appbase::ApplicationSettings settings;
		settings.title = "Test App";

		appbase::graph::GraphicsSettings graph_settings;
		graph_settings.clear.b = 0x80;

		appbase::sound::SoundSettings sound_settings;

		auto app = std::make_unique<MyApp>(settings,
			graph_settings, sound_settings);
		app->Load();
		app->Run();
	}
	catch (std::exception &error) {
		SDL_LogCritical(SDL_LOG_CATEGORY_APPLICATION,
			"%s", error.what());
		return EXIT_FAILURE;
	}
	catch (...) {
		// If main is replaced with SDL_main, SDL_main is C linkage.
		SDL_LogCritical(SDL_LOG_CATEGORY_APPLICATION,
			"Caught unknown exception");
		return EXIT_FAILURE;
	}
	return 0;
}
