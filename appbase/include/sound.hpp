#pragma once

#include <SDL_mixer.h>
#include "safeptr.hpp"

namespace appbase {
namespace sound {

struct SoundSettings {
	int frequency = MIX_DEFAULT_FREQUENCY;	// 22.05 Hz
	Uint16 format = MIX_DEFAULT_FORMAT;
	int channels = MIX_DEFAULT_CHANNELS;	// 2: stereo
	int chunksize = 4096;
	struct {
		bool flac = false;
		bool mod = false;
		bool mp3 = false;
		bool ogg = true;
	} load_enable;
};

class SoundManager final {
public:
	SoundManager(const SoundManager &) = delete;
	SoundManager & operator =(const SoundManager &) = delete;

	explicit SoundManager(const SoundSettings &settings);
	~SoundManager() = default;

private:
	SoundSettings m_settings;

	SdlMixerPtr m_sdl_mixer;
	SdlAudioPtr m_audio;
};

}
}
