#include "services/RootService.hpp"

#include "services/EntityService.hpp"
#include "services/SoundService.hpp"

using namespace service;

RootService::RootService(core::AssetManager *assets, state::App *state)
    : entities(std::make_unique<EntityService>(assets, state)), sounds(std::make_unique<SoundService>(assets, state)),
      assets(assets), state(state)
{
}

std::unique_ptr<RootService> &service::root(core::AssetManager *assets, state::App *state)
{
    // Initialized on first call (thread-safe in C++11+)
    static std::unique_ptr<RootService> instance = std::make_unique<RootService>(assets, state);
    return instance;
};