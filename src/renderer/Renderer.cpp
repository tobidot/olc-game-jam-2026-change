#include "renderer/Renderer.hpp"

using namespace renderer;

void Renderer::Draw(olc::Draw &draw, const core::AssetManager &assets, const state::App &state) const
{
    ClearBackground(draw);
    game_world.Draw(draw, assets, state);
    hud.Draw(draw, assets, state);
    ui.Draw(draw, assets, state);
}

void Renderer::ClearBackground(olc::Draw &draw) const
{
    olc::Pixel background_color{12, 44, 111};
    draw.Clear(background_color);
}