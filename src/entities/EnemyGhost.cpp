#include "entities/EnemyGhost.hpp"

#include "helper.hpp"
#include "plans/BasePlan.hpp"
#include "plans/DirectAttack.hpp"
#include "plans/Move.hpp"

using namespace entity;

EnemyGhost::EnemyGhost(const core::AssetManager &assets, const state::App &state, size_t id) : EnemyBase(id)
{
    health = max_health = 25.0f;
    animator = *assets.ghost_animator;
    shape.size = {10.f, 10.f};
    scale = {.45f, .45f};
    z_offset = 15.0f;
    current_animation = "walk";
    current_animation_time = 0.0f;
    can_fly = true;
}

/**
 * Ghost Moves fast and
 */
void EnemyGhost::MakeNextPlan(state::App &state, float elapsed_time)
{
    const auto attack_range = 25.0f;
    auto difference = state.game_world.player_entity->ref->position - position;
    auto distance = difference.mag();

    if (distance < attack_range)
    {
        const auto *attack_name = "attack1";
        auto duration = 0.5f;
        auto hit_time_window = animator.GetFrameWindowTime(attack_name, 4, 4) /
                               animator.GetAnimationSpeedForDuration(attack_name, duration);
        plan = std::make_unique<plan::DirectAttack>(plan::DirectAttackConfig{
            .target = state.game_world.player_entity,
            .animation_name = attack_name,
            .hit_time_window = hit_time_window,
            .duration = duration,
            .max_range = attack_range * 1.5f,
            .damage = 7.0f,
        });
        SetPlan(state, std::move(plan), 0.5f);
    }
    else
    {
        auto target = position + difference.norm() * std::min(100.0f, difference.mag());
        auto new_plan = std::make_unique<plan::Move>(core::Vector(target), 50.f);
        SetPlan(state, std::move(new_plan), .25f);
    }
}