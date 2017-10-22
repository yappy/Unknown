#include <application.hpp>
#include <array>
#include <string>
#include <vector>

class MyApp : public appbase::Application {
public:
	// inheriting constructor
	using Application::Application;

	void Update() override
	{}
	void Render() override
	{}
};

int main()
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

		appbase::graph::GraphicsSettings graphSettings;
		graphSettings.clear.b = 0x80;

		auto app = std::make_unique<MyApp>(settings, graphSettings);
		app->Graph()->LoadTexture(
			appbase::file::FromBasePath("res/sample_8_index.png"));
		app->Run();
	}
	catch (appbase::error::SDLError &error) {
		SDL_LogCritical(SDL_LOG_CATEGORY_APPLICATION,
			"%s", error.what());
		return 1;
	}
	return 0;
}
