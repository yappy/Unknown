#include <application.hpp>
#include <array>
#include <string>

int main()
{
	auto base = std::string(::SDL_GetBasePath());
	auto textpath = base + "res/test.txt";
	SDL_Log("path: %s", textpath.c_str());

	SDL_RWops *ops = SDL_RWFromFile(textpath.c_str(), "rb");
	if (ops != nullptr) {
		// 0-filled (default construct)
		std::array<uint8_t, 256> buf;
		SDL_RWread(ops, buf.data(), 1, buf.size() - 1);
		SDL_Log("%s", buf.data());
		SDL_RWclose(ops);
	}
	else {
		SDL_Log("File not found");
	}

	appbase::Application app("Test App");
	app.Run();

	return 0;
}
