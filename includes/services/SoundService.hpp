#pragma once
#include "core/AssetManager.hpp"
#include "olc/miniaudio.h"
#include "olc/olcPGEX3_Miniaudio.h"
#include "state/App.hpp"

namespace service
{

class SoundService
{
private:
    const core::AssetManager *assets;
    const state::App *state;

public:
    SoundService(const core::AssetManager *assets, const state::App *state);
    virtual ~SoundService() = default;

public:
    void Play(olc::ext::Miniaudio::Sound &sound);
};

}; // namespace service