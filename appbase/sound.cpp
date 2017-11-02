#include <sound.hpp>
#include <exceptions.hpp>

namespace appbase {
namespace sound {

using namespace appbase::error;

SoundManager::SoundManager(const SoundSettings &settings) :
	m_settings(settings)
{
	{
	::SDL_Log("Initialize SDL_mixer...");
		int flags = 0;
		if (settings.load_enable.flac) {
			flags |= MIX_INIT_FLAC;
		}
		if (settings.load_enable.mod) {
			flags |= MIX_INIT_MOD;
		}
		if (settings.load_enable.mp3) {
			flags |= MIX_INIT_MP3;
		}
		if (settings.load_enable.ogg) {
			flags |= MIX_INIT_OGG;
		}
		int inited = Mix_Init(flags);
		if ((inited & flags) != flags) {
			ThrowLastSdlMixerError();
		}
		m_sdl_mixer.reset(this);
		::SDL_Log("Initialize SDL_mixer OK");
	}
	{
		::SDL_Log("Open audio...");
		int flags = 0;
		if (Mix_OpenAudio(settings.frequency, settings.format,
			settings.channels, settings.chunksize) < 0) {
				ThrowLastSdlMixerError();
		}
		m_audio.reset(this);
		::SDL_Log("Open audio OK");
	}
}

}
}
