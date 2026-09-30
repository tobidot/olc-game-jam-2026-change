#include "entities/EnemyBase.hpp"

#include "plans/BasePlan.hpp"
#include "plans/DirectAttack.hpp"
#include "plans/Move.hpp"

#include <memory>

using namespace entity;

void EnemyBase::Update(state::App &state, float elapsed_time)
{
    Entity::Update(state, elapsed_time);

    if (plan)
    {
        plan->Update(state, *this, elapsed_time);
    }
    else
    {
        if (plan_cooldown <= 0.f)
        {
            auto difference = ((*state.game_world.player_entity)->position - position);
            auto distance = difference.mag();

            if (distance < 100.0f)
            {
                // after attackin add a second of delay
                plan_cooldown = 2.5f;
                //
                plan = std::make_unique<plan::DirectAttack>(plan::DirectAttackConfig{
                    .target = state.game_world.player_entity,
                    .animation_name = "attack1",
                    .hit_time = 0.1f,
                    .hit_time_window = 0.25f,
                    .duration = 1.0f,
                    .max_range = 125.0f,
                    .damage = 10.0f,
                });
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
        else
        {
            plan_cooldown -= elapsed_time;
        }
    }
}