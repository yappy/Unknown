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
	::Mix_AllocateChannels(settings.mixing_channels);
	m_playing_chunks = std::make_unique<SdlChunkSharedPtr[]>(
		settings.mixing_channels);
}

SoundManager::~SoundManager()
{
	if (m_audio != nullptr) {
		StopAllSe();
		StopBgm();
	}
}

void SoundManager::ProcessFrame()
{
	// se
	for (int i = 0; i < m_settings.mixing_channels; i++) {
		if (m_playing_chunks[i] != nullptr && !::Mix_Playing(i)) {
			// set nullptr and decrement refcount
			m_playing_chunks[i].reset();
		}
	}
	// bgm
	if (m_playing_bgm != nullptr && !::Mix_PlayingMusic()) {
		// set nullptr and decrement refcount
		m_playing_bgm.reset();
	}
}

bool SoundManager::PlaySe(const SdlChunkSharedPtr &se)
{
	for (int i = 0; i < m_settings.mixing_channels; i++) {
		if (m_playing_chunks[i] == nullptr) {
			if (::Mix_PlayChannel(i, se.get()->get(), 0) < 0) {
				ThrowLastSdlMixerError();
			}
			// assign and increment refcount
			m_playing_chunks[i] = se;
			return true;
		}
	}
	return false;
}

void SoundManager::StopAllSe()
{
	::Mix_HaltChannel(-1);
	ProcessFrame();
}

void SoundManager::PlayBgm(const SdlMusicSharedPtr &bgm, bool loop)
{
	int count = loop ? -1 : 1;
	if (::Mix_PlayMusic(bgm.get()->get(), count) < 0) {
		ThrowLastSdlMixerError();
	}
	// assign and increment refcount
	m_playing_bgm = bgm;
}

void SoundManager::StopBgm()
{
	::Mix_HaltMusic();
	ProcessFrame();
}

SdlChunkSharedPtr SoundManager::LoadSe(const std::string &path)
{
	auto chunk = SdlChunkPtr(::Mix_LoadWAV(path.c_str()));
	if (chunk == nullptr) {
		ThrowLastSdlMixerError();
	}
	return std::make_shared<SdlChunkPtr>(std::move(chunk));
}

SdlMusicSharedPtr SoundManager::LoadBgm(const std::string &path)
{
	auto music = SdlMusicPtr(::Mix_LoadMUS(path.c_str()));
	if (music == nullptr) {
		ThrowLastSdlMixerError();
	}
	return std::make_shared<SdlMusicPtr>(std::move(music));
}

}
}
