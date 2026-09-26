#pragma once
#include "core/AssetManager.hpp"
#include "olc/olcPixelGameEngine3.h"
#include "state/App.hpp"

namespace renderer
{

class UiRenderer
{
public:
    void Draw(olc::Draw &draw, const core::AssetManager &assets, const state::App &state) const;
};

} // namespace renderer