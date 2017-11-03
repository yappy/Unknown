#pragma once

#include <SDL_mixer.h>
#include "safeptr.hpp"
#include <string>

namespace appbase {
namespace sound {

struct SoundSettings {
	int frequency = MIX_DEFAULT_FREQUENCY;	// 22.05 Hz
	Uint16 format = MIX_DEFAULT_FORMAT;
	int channels = MIX_DEFAULT_CHANNELS;	// 2: stereo
	int chunksize = 4096;

	int mixing_channels = 16;

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
	~SoundManager();

	// Release reference if playing is finished
	void ProcessFrame();

	bool PlaySe(const SdlChunkSharedPtr &se);
	void StopAllSe();

	void PlayBgm(const SdlMusicSharedPtr &bgm, bool loop = true);
	void StopBgm();

	// SoundManager also keeps reference to the sound buffer while playing
	SdlChunkSharedPtr LoadSe(const std::string &path);
	SdlMusicSharedPtr LoadBgm(const std::string &path);

private:
	SoundSettings m_settings;

	SdlMixerPtr m_sdl_mixer;
	SdlAudioPtr m_audio;

	std::unique_ptr<SdlChunkSharedPtr[]> m_playing_chunks;
	SdlMusicSharedPtr m_playing_bgm;
};

}
}
