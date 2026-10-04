#pragma once
#include "core/AssetManager.hpp"
#include "entities/Effect.hpp"
#include "state/App.hpp"

namespace entity
{

class EffectPiercingLight : public Effect
{
public:
    float damage = 60.0f;
    std::unordered_map<size_t, std::shared_ptr<entity::EntityHandle>> has_hit_enemies;

public:
    EffectPiercingLight(const core::AssetManager &assets, const state::App &state, size_t id);
    ~EffectPiercingLight() override;

public:
    void Update(state::App &state, float elapsed_time) override;
    void MakeNextPlan(state::App &state, float elapsed_time) override;
};

} // namespace entity