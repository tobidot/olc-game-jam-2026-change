#pragma once
#include "core/AssetManager.hpp"
#include "entities/Effect.hpp"
#include "state/App.hpp"

namespace entity
{

class EffectFireBall : public Effect
{
public:
    EffectFireBall(const core::AssetManager &assets, const state::App &state, size_t id);

public:
    void Update(state::App &state, float elapsed_time) override;
    void MakeNextPlan(state::App &state, float elapsed_time) override;
};

} // namespace entity