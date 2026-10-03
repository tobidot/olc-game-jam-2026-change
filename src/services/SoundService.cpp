#include "services/SoundService.hpp"

#include "core/AssetManager.hpp"
#include "exceptions/Exceptions.hpp"
#include "services/EntityService.hpp"

#include <memory>

using namespace service;

SoundService::SoundService(const core::AssetManager *assets, const state::App *state) : assets(assets), state(state)
{
}

void SoundService::Play(olc::ext::Miniaudio::Sound &sound)
{
    sound.Play();
}