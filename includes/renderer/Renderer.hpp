#pragma once

#include "olc/olcPixelGameEngine3.h"
#include "renderer/GameWorldRenderer.hpp"
#include "renderer/HudRenderer.hpp"
#include "renderer/UiRenderer.hpp"
#include "state/App.hpp"

namespace renderer
{

class Renderer
{
public:
    GameWorldRenderer game_world;
    HudRenderer hud;
    UiRenderer ui;

public:
    void Draw(olc::Draw &draw, const core::AssetManager &assets, const state::App &state) const;
    void ClearBackground(olc::Draw &draw) const;
};

} // namespace renderer
