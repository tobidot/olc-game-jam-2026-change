#pragma once

#include "core/AssetManager.hpp"
#include "entities/Entity.hpp"
#include "entities/PlayerBase.hpp"
#include "state/App.hpp"

namespace entity
{

class PlayerShinobi : public PlayerBase
{
public:
    PlayerShinobi(const core::AssetManager &assets, const state::App &state, size_t id);
    ~PlayerShinobi() override = default;

public:
    void Update(state::App &state, float elapsed_time) override;
    void MakeNextPlan(state::App &state, float elapsed_time) override;
};

} // namespace entity