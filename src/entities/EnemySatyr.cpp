#include "entities/EnemySatyr.hpp"

using namespace entity;

EnemySatyr::EnemySatyr(const core::AssetManager &assets, const state::App &state)
{
    health = max_health = 10.0f;
    animator = *assets.enemy_satyr_animator;
    scale = {.75f, .75f};
    current_animation = "walk";
    current_animation_time = 0.0f;
}