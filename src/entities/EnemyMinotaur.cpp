#include "entities/EnemyMinotaur.hpp"

#include "helper.hpp"
#include "plans/DirectAttack.hpp"
#include "plans/Heal.hpp"
#include "plans/Move.hpp"

using namespace entity;

EnemyMinotaur::EnemyMinotaur(const core::AssetManager &assets, const state::App &state, size_t id) : EnemyBase(id)
{
    health = max_health = 225.0f;
    animator = *assets.minotaur_animator;
    shape.size = {25.f, 25.f};
    scale = {1.f, 1.f};
    current_animation = "walk";
    current_animation_time = 0.0f;
}

/**
 * Minotaur are heavy and regenerate
 */
void EnemyMinotaur::MakeNextPlan(state::App &state, float elapsed_time)
{
    const auto attack_range = 45.0f;
    auto difference = state.game_world.player_entity->ref->position - position;
    auto distance = difference.mag();

    if (distance < attack_range)
    {
        const auto *attack_name = "attack1";
        auto duration = 1.5f;
        auto hit_time_window = animator.GetFrameWindowTime(attack_name, 4, 4) /
                               animator.GetAnimationSpeedForDuration(attack_name, duration);
        plan = std::make_unique<plan::DirectAttack>(plan::DirectAttackConfig{
            .target = state.game_world.player_entity,
            .animation_name = attack_name,
            .hit_time_window = hit_time_window,
            .duration = duration,
            .max_range = attack_range * 1.5f,
            .damage = 18.0f,
        });
        SetPlan(state, std::move(plan), 0.75f);
    }
    else if (health < max_health * 0.6f)
    {
        auto self = state.game_world.FindEntityHandle(id);
        auto duration = 0.5f;
        auto heal_time_window =
            animator.GetFrameWindowTime("idle", 3, 3) / animator.GetAnimationSpeedForDuration("idle", duration);

        plan = std::make_unique<plan::Heal>(plan::HealConfig{
            .target = self,
            .animation_name = "idle",
            .heal_time_window = heal_time_window,
            .duration = duration,
            .amount = 35.0f,
        });
        SetPlan(state, std::move(plan), 0.5f);
    }
    else
    {
        auto target = position + difference.norm() * std::min(40.0f, difference.mag());
        auto new_plan = std::make_unique<plan::Move>(core::Vector(target), 60.f);
        SetPlan(state, std::move(new_plan), .5f);
    }
}