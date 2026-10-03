#pragma once

#include <memory>

namespace core
{
class AssetManager;
}; // namespace core

namespace state
{
class App;
};

namespace service
{

class EntityService;
class SoundService;

class RootService
{
public:
    core::AssetManager *assets;
    state::App *state;

public:
    std::unique_ptr<EntityService> entities;
    std::unique_ptr<SoundService> sounds;

public:
    RootService(core::AssetManager *assets, state::App *state);
};

std::unique_ptr<RootService> &root(core::AssetManager *assets = nullptr, state::App *state = nullptr);

}; // namespace service