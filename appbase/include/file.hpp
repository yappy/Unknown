#pragma once

#include <SDL.h>
#include <memory>
#include <string>
#include <vector>
#include <map>
#include "safeptr.hpp"

namespace appbase {
namespace file {

using Bytes = std::vector<uint8_t>;
using Lines = std::vector<std::string>;

// 256MiB
const size_t DefaultMaxSize = 256 * 1024 * 1024;

const std::string &GetBasePath();
inline std::string FromBasePath(const char *path)
{
	return GetBasePath() + path;
}
FilePointer OpenR(const std::string &path);
FilePointer OpenW(const std::string &path);
Bytes ReadAllBytes(const FilePointer &fp, size_t maxsize = DefaultMaxSize);
std::string ReadAllString(const FilePointer &fp, size_t maxsize = DefaultMaxSize);
Lines ReadAllLines(const FilePointer &fp, size_t maxsize = DefaultMaxSize);
void WriteAllBytes(const FilePointer &fp, const void *buf, size_t size);

// would like to use std::variant...
struct ConfigElement final {
	enum class Type {
		String, Bool, Int,
	};

	Type type;
	std::string stringValue;
	bool boolValue;
	int intValue;

	// allow implicit construct
	ConfigElement(const char *s) : type(Type::String), stringValue(s) {}
	ConfigElement(bool b) : type(Type::Bool), boolValue(b) {}
	ConfigElement(int i) : type(Type::Int), intValue(i) {}
	~ConfigElement() = default;
};

class ConfigFile final {
private:
	std::map<std::string, ConfigElement> m_data;
public:
	ConfigFile(std::initializer_list<decltype(m_data)::value_type> defaultList) :
		m_data(defaultList)
	{}
	ConfigFile(const ConfigFile &) = default;
	ConfigFile & operator=(const ConfigFile &) = default;
	~ConfigFile() = default;

	void Load(const std::string &filepath);
	void Save(const std::string &filepath);
};

}
}
