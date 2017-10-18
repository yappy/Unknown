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

void WriteAllBytes(const FilePointer &fp, const void *buf, size_t size)
{
	size_t ret = SDL_RWwrite(fp.get(), buf, 1, size);
	if (ret != size) {
		ThrowLastSDLError<SDLFileError>();
	}
}

void ConfigFile::Load(const std::string &filepath)
{
	try {
		auto fp = OpenR(filepath);
		Lines lines = ReadAllLines(fp);
		int lineNo = 0;
		for (std::string line : lines) {
			lineNo++;
			if (line.empty() || line[0] == '#') {
				continue;
			}
			// TODO parse
		}
		// close
	}
	catch (SDLFileError &) {
		// read error: log and ignore
		SDL_Log("Load config file failed: %s", filepath.c_str());
	}
	// write fixed data
	try {
		Save(filepath);
	}
	catch (SDLFileError &) {
		// write error: log and rethrow
		SDL_Log("Recreate config file failed: %s", filepath.c_str());
		throw;
	}
}

void ConfigFile::Save(const std::string &filepath)
{
	auto fp = OpenW(filepath);
	for (const auto &kv : m_data) {
		std::string line = kv.first;
		line += '=';
		switch (kv.second.type) {
		case ConfigElement::Type::String:
			line += kv.second.stringValue;
			break;
		case ConfigElement::Type::Bool:
			line += kv.second.boolValue ? "true" : "false";
			break;
		case ConfigElement::Type::Int:
			line += std::to_string(kv.second.intValue);
			break;
		default:
			SDL_assert(0);
		}
		WriteAllBytes(fp, line.c_str(), line.size());
	}
	// close
}

}
}
