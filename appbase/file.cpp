#include <file.hpp>
#include <exceptions.hpp>
#include <string>

namespace appbase {
namespace file {

namespace {
	const size_t BufSize = 64 * 1024;

	// SDL_GetBasePath() cache
	// should be accessed via GetBasePath()
	std::string base_path;

	template <class T>
	T ReadAllInternal(const FilePointer &fp, size_t maxsize)
	{
		T result;
		std::array<typename T::value_type, BufSize> buf;
	
		SDL_ClearError();
		size_t read_size;
		while ((read_size = SDL_RWread(fp.get(), buf.data(), 1, buf.size())) != 0) {
			if (result.size() + read_size > maxsize) {
				throw SDLFileError("File size over");
			}
			result.insert(result.end(), buf.cbegin(), buf.cbegin() + read_size);
		}
		if (*::SDL_GetError()) {
			// read failed
			// SDL_RWread() returns 0 and SDL error is not empty string
			ThrowLastSDLError<SDLFileError>();
		}
		return result;
	}
}

const std::string &GetBasePath()
{
	if (base_path.empty()) {
		auto del = [](char *p) {
			::SDL_free(p);
		};
		auto p = std::unique_ptr<char, decltype(del)>(
			::SDL_GetBasePath(), del);
		if (p == nullptr) {
			// SDL_GetBasePath() failed
			ThrowLastSDLError<SDLFileError>();
		}
		base_path = p.get();
	}
	return base_path;
}

FilePointer OpenAtBaseR(const char *filepath)
{
	std::string base = GetBasePath();
	auto fullpath = base + filepath;
	auto result = FilePointer(
		SDL_RWFromFile(fullpath.c_str(), "rb"), RWopsDeleter);
	if (result == nullptr) {
		ThrowLastSDLError<SDLFileError>();
	}
	return result;
}

Bytes ReadAllBytes(const FilePointer &fp, size_t maxsize)
{
	return ReadAllInternal<Bytes>(fp, maxsize);
}

std::string ReadAllString(const FilePointer &fp, size_t maxsize)
{
	return ReadAllInternal<std::string>(fp, maxsize);
}


}
}
