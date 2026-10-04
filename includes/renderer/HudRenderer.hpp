#pragma once
#include "core/AssetManager.hpp"
#include "olc/olcPixelGameEngine3.h"
#include "state/App.hpp"

namespace renderer
{

class HudRenderer
{
public:
    void Draw(olc::Draw &draw, const core::AssetManager &assets, const state::App &state) const;

public:
    void DrawCharacterSlot(
        olc::Draw &draw,
        const core::AssetManager &assets,
        const olc::vf2d &center,
        enums::CharacterType next,
        float cooldown
    ) const;
    void FillArc(olc::Draw &draw, const olc::vf2d &center, float radius, float radians, const olc::Pixel &color) const;
};

} // namespace renderer