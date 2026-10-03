#include "entities/PlayerBase.hpp"

#include "enums/Enums.hpp"
#include "plans/DirectAttack.hpp"
#include "plans/Move.hpp"

using namespace entity;

PlayerBase::PlayerBase(enums::CharacterType type, size_t id) : Entity(id, enums::TargetType::PLAYER), type(type)
{
}

void PlayerBase::Update(state::App &state, float elapsed_time)
{
    Entity::Update(state, elapsed_time);
}

std::shared_ptr<EntityHandle> PlayerBase::FindClosestEnemyTo(state::App &state, core::Vector position)
{
    auto current_entity = std::shared_ptr<EntityHandle>(nullptr);
    auto current_distance2 = std::numeric_limits<float>::max();
    for (const auto &entity : state.game_world.entities)
    {
        if (entity->ref->target_type != enums::TargetType::ENEMY)
        {
            // ignore the player
            continue;
        }
        if (entity->ref->is_dying || entity->ref->is_removed)
        {
            // ignore dying or removed entities
            continue;
        }
        const auto distance2 = (entity->ref->position - position).mag2();
        if (distance2 < current_distance2)
        {
            current_entity = entity;
            current_distance2 = distance2;
        }
    }
    return current_entity;
}

void PlayerBase::MakeNextPlan(state::App &state, float elapsed_time)
{
    auto player = state.game_world.player_entity;

    auto enemy = [&]()
    {
        if (last_attack_target && last_attack_target->ref)
        {
            if (last_attack_target->ref->is_removed)
            {
                last_attack_target = std::shared_ptr<entity::EntityHandle>(nullptr);
            }
            else if (!last_attack_target->ref->is_dying)
            {
                return last_attack_target;
            }
        }

        return FindClosestEnemyTo(state, position);
    }();

    if (!enemy)
    {
        // move to the right to the end of the level
        // if no enemy is in sight
        auto target = player->ref->position + core::Vector{50.0f, 0.f};
        plan = std::make_unique<plan::Move>(core::Vector(target), 50.f);
        // after walking add a second of delay
        plan_cooldown = 0.01f;
        return;
    }

    auto difference = enemy->ref->position - player->ref->position;
    auto distance = difference.mag();

    if (distance > 60.0f)
    {
        // move towards the enemy
        auto target = player->ref->position + difference.norm() * std::min(25.0f, distance);
        plan = std::make_unique<plan::Move>(core::Vector(target), 45.f);
        // after walking add a second of delay
        plan_cooldown = 0.0f;
        return;
    }

    //
    plan = std::make_unique<plan::DirectAttack>(plan::DirectAttack::FromAnimationFrame(
        plan::DirectAttackFromAnimationFrameConfig{
            .source = player,
            .target = enemy,
            .animation_name = "attack1",
            .hit_frame = 4,
            .duration = 0.66f,
            .max_range = 125.0f,
            .damage = 40.0f,
        }
    ));
    last_attack_target = enemy;
    // after attackin add a second of delay
    SetPlan(state, std::move(plan), 1.75f);
}