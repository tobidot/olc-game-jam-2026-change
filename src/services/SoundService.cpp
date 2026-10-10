#include "services/SoundService.hpp"

#include "core/AssetManager.hpp"
#include "exceptions/Exceptions.hpp"
#include "services/EntityService.hpp"

#include <memory>

using namespace service;

SoundService::SoundService(const core::AssetManager *assets, const state::App *state) : assets(assets), state(state)
{
}

void SoundService::PlayMusic(olc::ext::Miniaudio::Sound &sound)
{
    sound.SetVolume(state->settings.music_volume);
    sound.Play();
}

void SoundService::Play(olc::ext::Miniaudio::Sound &sound)
{
    sound.SetVolume(state->settings.sfx_volume);
    sound.Play();
}