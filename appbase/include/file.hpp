#pragma once

#include <SDL2/SDL.h>
#include <memory>
#include <string>
#include <vector>

namespace appbase {
namespace file {

inline void RWopsDeleter(SDL_RWops *p) noexcept
{
	SDL_RWclose(p);
}

using FilePointer = std::unique_ptr<SDL_RWops, decltype(&RWopsDeleter)>;

using Bytes = std::vector<uint8_t>;
using Lines = std::vector<std::string>;

// 256MiB
const size_t DefaultMaxSize = 256 * 1024 * 1024;

const std::string &GetBasePath();
FilePointer OpenAtBaseR(const char *filepath);
Bytes ReadAllBytes(const FilePointer &fp, size_t maxsize = DefaultMaxSize);
std::string ReadAllString(const FilePointer &fp, size_t maxsize = DefaultMaxSize);
Lines ReadAllLines(const FilePointer &fp, size_t maxsize = DefaultMaxSize);

}
}
