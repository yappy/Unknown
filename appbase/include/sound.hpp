#pragma once

#include <SDL_mixer.h>
#include "safeptr.hpp"

namespace appbase {
namespace sound {


class SoundManager final {
public:
	SoundManager(const SoundManager &) = delete;
	SoundManager & operator =(const SoundManager &) = delete;

	SoundManager() = default;
	~SoundManager() = default;

private:

};

}
}
