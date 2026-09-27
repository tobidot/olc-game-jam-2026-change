#include "entities/EnemyGhost.hpp"

using namespace entity;

EnemyGhost::EnemyGhost(const core::AssetManager &assets, const state::App &state)
{
    health = max_health = 10.0f;
    animator = *assets.enemy_ghost_animator;
    scale = {.75f, .75f};
    current_animation = "walk";
    current_animation_time = 0.0f;
}