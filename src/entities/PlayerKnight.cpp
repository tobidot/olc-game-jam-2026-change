#include "entities/PlayerKnight.hpp"

#include "core/AssetManager.hpp"

using namespace entity;

PlayerKnight::PlayerKnight(const core::AssetManager &assets, const state::App &state)
{
    health = max_health = 100.0f;
    animator = *assets.hero_knight_animator;
    scale = {1.f, 1.f};
    current_animation = "walk";
    current_animation_time = 0.0f;
}

void PlayerKnight::Update(state::App &state, float elapsed_time)
{
    Entity::Update(state, elapsed_time);

    position.x += elapsed_time * 30.0f;
}