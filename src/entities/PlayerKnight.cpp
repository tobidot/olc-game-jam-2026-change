#include "entities/PlayerKnight.hpp"

#include "core/AssetManager.hpp"

using namespace entity;

PlayerKnight::PlayerKnight(const core::AssetManager &assets, const state::App &state, size_t id)
    : PlayerBase(enums::CharacterType::KNIGHT, id)
{
    health = max_health = 500.0f;
    animator = *assets.knight_animator;
    scale = {1.f, 1.f};
    current_animation = "walk";
    current_animation_time = 0.0f;
}

void PlayerKnight::Update(state::App &state, float elapsed_time)
{
    PlayerBase::Update(state, elapsed_time);
}