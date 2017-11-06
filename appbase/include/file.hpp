#pragma once

#include <SDL.h>
#include <memory>
#include <string>
#include <vector>
#include <map>
#include <functional>
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


struct StringLenCheck {
	static const size_t DefaultLen = 256;

	explicit StringLenCheck(size_t len = DefaultLen) : m_len(len) {}
	bool operator ()(const std::string &v)
	{
		return v.size() <= m_len;
	}
private:
	size_t m_len;
};

struct IntRangeCheck {
	IntRangeCheck(int min, int max) : m_min(min), m_max(max)
	{
		if (min > max) {
			throw std::invalid_argument("Invalid range");
		}
	}
	bool operator ()(int v)
	{
		return v >= m_min && v <= m_max;
	}
private:
	int m_min, m_max;
};

// would like to use std::variant...
struct ConfigElement final {
	enum class Type {
		String, Bool, Int,
	};

	Type type;

	std::string string_value;
	bool bool_value;
	int int_value;

	std::function<bool(const std::string &)> string_check;
	std::function<bool(int)> int_check;

	// allow implicit construct
	ConfigElement(const std::string &s,
		std::function<bool(const std::string &)> c = StringLenCheck()) :
		type(Type::String), string_value(s), string_check(c)
	{}
	ConfigElement(bool b) : type(Type::Bool), bool_value(b) {}
	ConfigElement(int i, std::function<bool(int)> c = nullptr) :
		type(Type::Int), int_value(i), int_check(c)
	{}

	// allow copy or move (default)
	ConfigElement(const ConfigElement &) = default;
	ConfigElement(ConfigElement &&) = default;
	ConfigElement & operator=(const ConfigElement &) = default;
	ConfigElement & operator=(ConfigElement &&) = default;

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

	std::string GetString(const std::string &key);
	bool GetBool(const std::string &key);
	int GetInt(const std::string &key);

	void SetString(const std::string &key, const std::string &value);
	void SetBool(const std::string &key, bool value);
	void SetInt(const std::string &key, int value);

	void Load(const std::string &filepath);
	void Save(const std::string &filepath);
};

}
}
