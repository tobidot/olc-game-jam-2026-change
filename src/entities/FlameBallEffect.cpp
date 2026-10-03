#include "core/AssetManager.hpp"
#include "entities/EffectFireBall.hpp"
#include "state/App.hpp"

using namespace entity;

EffectFireBall::EffectFireBall(const core::AssetManager &assets, const state::App &state, size_t id)
    : Effect(assets, state, id)
{
    health = max_health = 25.0f;
    animator = *assets.satyr_missle1_animator;
    shape.size = {10.f, 10.f};
    scale = {.45f, .45f};
    z_offset = 15.0f;
    current_animation = "walk";
    current_animation_time = 0.0f;
}

void EffectFireBall::Update(state::App &state, float elapsed_time)
{
    Effect::Update(state, elapsed_time);
}

void EffectFireBall::MakeNextPlan(state::App &state, float elapsed_time)
{
    Effect::MakeNextPlan(state, elapsed_time);
}