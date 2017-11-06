#include <file.hpp>
#include <exceptions.hpp>
#include <array>
#include <regex>

namespace appbase {
namespace file {

using namespace appbase::error;

namespace {
	// SDL_GetBasePath() cache
	// should be accessed via GetBasePath()
	std::string base_path;
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
			ThrowLastSdlError<SdlFileError>();
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
			ThrowLastSdlError<SdlFileError>();
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

namespace {
	const size_t BufSize = 64 * 1024;

	// Func(void *buf, size_t read_size)
	template <class Func>
	void ReadAll(const FilePointer &fp, Func func)
	{
		std::array<uint8_t, BufSize> buf;

		SDL_ClearError();
		size_t read_size;
		while ((read_size = SDL_RWread(fp.get(), buf.data(), 1, buf.size())) != 0) {
			func(static_cast<void *>(buf.data()), read_size);
		}
		if (*::SDL_GetError() != '\0') {
			// read failed
			// SDL_RWread() returns 0 and SDL error is not empty string
			ThrowLastSdlError<SdlFileError>();
		}
	}
}

Bytes ReadAllBytes(const FilePointer &fp, size_t maxsize)
{
	Bytes result;
	ReadAll(fp, [&result, maxsize](void *buf, size_t read_size) {
		if (result.size() + read_size > maxsize) {
			throw SdlFileError("File size over");
		}
		auto p = static_cast<uint8_t *>(buf);
		result.insert(result.end(), p, p + read_size);
	});

	return result;
}

std::string ReadAllString(const FilePointer &fp, size_t maxsize)
{
	std::string result;
	ReadAll(fp, [&result, maxsize](void *buf, size_t read_size) {
		if (result.size() + read_size > maxsize) {
			throw SdlFileError("File size over");
		}
		auto p = static_cast<char *>(buf);
		result.insert(result.end(), p, p + read_size);
	});
	if (result.find('\0') != std::string::npos) {
		throw SdlFileError("NUL detected in text file");
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
		ThrowLastSdlError<SdlFileError>();
	}
}

std::string ConfigFile::GetString(const std::string &key)
{
	const auto &v = m_data.at(key);
	if (v.type != ConfigElement::Type::String) {
		throw std::logic_error(key + " is not string");
	}
	return v.string_value;
}

bool ConfigFile::GetBool(const std::string &key)
{
	const auto &v = m_data.at(key);
	if (v.type != ConfigElement::Type::Bool) {
		throw std::logic_error(key + " is not bool");
	}
	return v.bool_value;
}

int ConfigFile::GetInt(const std::string &key)
{
	const auto &v = m_data.at(key);
	if (v.type != ConfigElement::Type::Int) {
		throw std::logic_error(key + " is not int");
	}
	return v.int_value;
}

void ConfigFile::SetString(const std::string &key, const std::string &value)
{
	auto &v = m_data.at(key);
	if (v.type != ConfigElement::Type::String) {
		throw std::logic_error(key + " is not string");
	}
	if (v.string_check != nullptr) {
		if (!v.string_check(value)) {
			SDL_Log("Illegal value: %s", value.c_str());
			return;
		}
	}
	v.string_value = value;
}

void ConfigFile::SetBool(const std::string &key, bool value)
{
	auto &v = m_data.at(key);
	if (v.type != ConfigElement::Type::Bool) {
		throw std::logic_error(key + " is not bool");
	}
	v.bool_value = value;
}

void ConfigFile::SetInt(const std::string &key, int value)
{
	auto &v = m_data.at(key);
	if (v.type != ConfigElement::Type::Int) {
		throw std::logic_error(key + " is not int");
	}
	if (v.int_check != nullptr) {
		if (!v.int_check(value)) {
			SDL_Log("Illegal value: %d", value);
			return;
		}
	}
	v.int_value = value;
}

void ConfigFile::Load(const std::string &filepath)
{
	SDL_Log("Load config file: %s", filepath.c_str());
	try {
		auto fp = OpenR(filepath);
		Lines lines = ReadAllLines(fp);

		std::regex re("([\\w\\.]+)=(.*)");
		std::smatch m;
		int lineNo = 0;
		for (std::string line : lines) {
			lineNo++;
			// empty line cr comment: skip
			if (line.empty() || line[0] == '#') {
				continue;
			}
			// line match
			if (std::regex_match(line, m, re)) {
				std::string key = m.str(1);
				std::string value = m.str(2);
				auto it = m_data.find(key);
				if (it != m_data.end()) {
					switch (it->second.type) {
					case ConfigElement::Type::String:
						SetString(key, value);
						break;
					case ConfigElement::Type::Bool:
						if (value == "true") {
							SetBool(key, true);
						}
						else if (value == "false") {
							SetBool(key, false);
						}
						else {
							SDL_Log("Illegal key: %s (%d)", key.c_str(), lineNo);
						}
						break;
					case ConfigElement::Type::Int:
						try {
							SetInt(key, std::stoi(value));
						}
						catch (...) {
							SDL_Log("Illegal int value: %s (%d)", value.c_str(), lineNo);
						}
						break;
					default:
						SDL_assert(0);
					}
				}
				else {
					SDL_Log("Illegal key: %s (%d)", key.c_str(), lineNo);
				}
			}
			else {
				SDL_Log("Illegal line %d: %s", lineNo, line.c_str());
			}
		}
		// close
	}
	catch (SdlFileError &) {
		// read error: log and ignore
		SDL_Log("Load config file failed: %s", filepath.c_str());
	}
	// write fixed data
	try {
		Save(filepath);
	}
	catch (SdlFileError &) {
		// write error: log and rethrow
		SDL_Log("Recreate config file failed: %s", filepath.c_str());
		throw;
	}
}

void ConfigFile::Save(const std::string &filepath)
{
	SDL_Log("Save config file: %s", filepath.c_str());
	auto fp = OpenW(filepath);
	for (const auto &kv : m_data) {
		std::string line = kv.first;
		line += '=';
		switch (kv.second.type) {
		case ConfigElement::Type::String:
			line += kv.second.string_value;
			break;
		case ConfigElement::Type::Bool:
			line += kv.second.bool_value ? "true" : "false";
			break;
		case ConfigElement::Type::Int:
			line += std::to_string(kv.second.int_value);
			break;
		default:
			SDL_assert(0);
		}
		line += '\n';
		WriteAllBytes(fp, line.c_str(), line.size());
	}
	// close
}

}
}
