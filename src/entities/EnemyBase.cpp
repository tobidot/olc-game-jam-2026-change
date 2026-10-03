#include "entities/EnemyBase.hpp"

#include "helper.hpp"
#include "plans/BasePlan.hpp"
#include "plans/DirectAttack.hpp"
#include "plans/Move.hpp"

#include <memory>

using namespace entity;

EnemyBase::EnemyBase(size_t id) : Entity(id, enums::TargetType::ENEMY)
{
}

void EnemyBase::MakeNextPlan(state::App &state, float elapsed_time)
{
    auto attack_range = 100.0f;
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
            .animation_name = "attack1",
            .hit_time_window = hit_time_window,
            .duration = 1.5f,
            .max_range = attack_range * 1.5f,
            .damage = 18.0f,
        });
        SetPlan(state, std::move(plan), 0.75f);
    }
    else
    {
        // after walking add a second of delay
        plan_cooldown = 1.5f;
        // move 75.f into player direction
        auto target = position + difference.norm() * std::min(75.0f, difference.mag());
        plan = std::make_unique<plan::Move>(core::Vector(target), 40.f);
    }
}