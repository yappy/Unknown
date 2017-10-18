#include <file.hpp>
#include <exceptions.hpp>

using appbase::error::SDLFileError;
using appbase::error::ThrowLastSDLError;

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

namespace {
	FilePointer OpenInternal(const std::string &path, const char *mode)
	{
		auto result = FilePointer(SDL_RWFromFile(path.c_str(), mode));
		if (result == nullptr) {
			ThrowLastSDLError<SDLFileError>();
		}
		return result;
	}
}

FilePointer OpenR(const std::string &path)
{
	return OpenInternal(path, "rb");
}

FilePointer OpenW(const std::string &path)
{
	return OpenInternal(path, "wb");
}

Bytes ReadAllBytes(const FilePointer &fp, size_t maxsize)
{
	return ReadAllInternal<Bytes>(fp, maxsize);
}

std::string ReadAllString(const FilePointer &fp, size_t maxsize)
{
	auto result = ReadAllInternal<std::string>(fp, maxsize);
	if (result.find('\0') != std::string::npos) {
		throw SDLFileError("NUL detected in text file");
	}
	return result;
}

Lines ReadAllLines(const FilePointer &fp, size_t maxsize)
{
	Lines result;
	std::string org = ReadAllString(fp, maxsize);

	decltype(org.find('\n', 0)) start = 0;
	while (start < org.size()) {
		decltype(start) end = 0;
		auto lfpos = org.find('\n', start);
		if (lfpos == std::string::npos) {
			end = org.size();
			if (start == end) {
				break;
			}
		}
		else {
			end = lfpos;
		}
		result.emplace_back(org, start, end - start);
		start = end + 1;
	}
	return result;
}



}
}
