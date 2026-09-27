#pragma once
#include "core/AssetManager.hpp"
#include "entities/Entity.hpp"
#include "olc/olcPixelGameEngine3.h"
#include "state/App.hpp"

namespace renderer
{

class GameWorldRenderer
{
public:
    void Draw(olc::Draw &draw, const core::AssetManager &assets, const state::App &state) const;
    void DrawBackground(olc::Draw &draw, const core::AssetManager &assets, const state::App &state) const;
    void DrawEntity(
        olc::Draw &draw, const core::AssetManager &assets, const state::App &state, const entity::Entity &entity
    ) const;
};

} // namespace renderer