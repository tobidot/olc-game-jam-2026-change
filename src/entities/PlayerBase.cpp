#include "entities/PlayerBase.hpp"

#include "enums/Enums.hpp"
#include "plans/DirectAttack.hpp"
#include "plans/Move.hpp"

using namespace entity;

PlayerBase::PlayerBase(enums::CharacterType type) : type(type)
{
}

void PlayerBase::Update(state::App &state, float elapsed_time)
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
            DetermineNextPlan(state, elapsed_time);
        }
        else
        {
            plan_cooldown -= elapsed_time;
        }
    }
}

EntityHandle PlayerBase::FindClosestEnemyTo(state::App &state, core::Vector position)
{
    auto current_entity = EntityHandle(nullptr);
    auto current_distance2 = std::numeric_limits<float>::max();
    for (const auto &entity : state.game_world.entities)
    {
        if (*entity == *state.game_world.player_entity)
        {
            // ignore the player
            continue;
        }
        const auto distance2 = ((*entity)->position - position).mag2();
        if (distance2 < current_distance2)
        {
            current_entity = entity;
            current_distance2 = distance2;
        }
    }
    return current_entity;
}

void PlayerBase::DetermineNextPlan(state::App &state, float elapsed_time)
{
    auto player = state.game_world.player_entity;
    auto enemy = FindClosestEnemyTo(state, (*player)->position);

    if (!enemy)
    {
        // move to the right to the end of the level
        // if no enemy is in sight
        auto target = (*player)->position + core::Vector{50.0f, 0.f};
        plan = std::make_unique<plan::Move>(core::Vector(target), 50.f);
        // after walking add a second of delay
        plan_cooldown = 0.01f;
        return;
    }

    auto difference = ((*enemy)->position - (*player)->position);
    auto distance = difference.mag();

    if (distance > 100.0f)
    {
        // move towards the enemy
        auto target = (*player)->position + difference.norm() * std::min(25.0f, distance);
        plan = std::make_unique<plan::Move>(core::Vector(target), 75.f);
        // after walking add a second of delay
        plan_cooldown = 0.0f;
        return;
    }

    //
    plan = std::make_unique<plan::DirectAttack>(plan::DirectAttackConfig{
        .target = enemy,
        .animation_name = "attack1",
        .hit_time = 0.1f,
        .hit_time_window = 0.25f,
        .duration = 0.66f,
        .max_range = 125.0f,
        .damage = 40.0f,
    });
    // after attackin add a second of delay
    plan_cooldown = .5f;
}