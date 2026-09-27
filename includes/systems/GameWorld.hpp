#pragma once
#include "core/AssetManager.hpp"
#include "core/Geometry.hpp"
#include "olc/olcPixelGameEngine3.h"
#include "state/App.hpp"
#include "systems/GameInput.hpp"

#include <memory>
#include <vector>

namespace systems
{

class GameWorld
{

public:
    core::RectF boundaries = {
        .top = 0.f,
        .left = 0.f,
        .bottom = 100.f,
        .right = 100.f,
    };

public:
    void Load(const core::AssetManager &assets, state::App &state);
    void Seed(int generator_seed, const core::AssetManager &asset_manager);
    void Update(const GameInput &input, state::App &state, float elapsed_time);
    void HandleWorldBounds(state::App &state, entity::Entity &entity, float elapsed_time);
};

} // namespace systems