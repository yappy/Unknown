#include <application.hpp>
#include <array>
#include <string>

int main()
{
	try {
		auto fp = appbase::file::OpenAtBaseR("res/test.txt");
		std::string strdata = appbase::file::ReadAllString(fp);
		SDL_Log(strdata.c_str());

		appbase::Application app("Test App");
		app.Run();
	}
	catch (appbase::SDLError &error) {
		SDL_LogCritical(SDL_LOG_CATEGORY_APPLICATION,
			"%s", error.what());
		return 1;
	}
	return 0;
}
