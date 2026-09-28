#include "entities/PlayerSamurai.hpp"

#include "core/AssetManager.hpp"

using namespace entity;

PlayerSamurai::PlayerSamurai(const core::AssetManager &assets, const state::App &state)
    : PlayerBase(enums::CharacterType::SAMURAI)
{
    health = max_health = 200.0f;
    animator = *assets.samurai_animator;
    scale = {1.f, 1.f};
    current_animation = "walk";
    current_animation_time = 0.0f;
}

void PlayerSamurai::Update(state::App &state, float elapsed_time)
{
    Entity::Update(state, elapsed_time);

    position.x += elapsed_time * 30.0f;
}