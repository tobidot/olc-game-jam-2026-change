#include "entities/PlayerBase.hpp"

#include "enums/Enums.hpp"
#include "helper.hpp"
#include "plans/DirectAttack.hpp"
#include "plans/Move.hpp"

#include <functional>

using namespace entity;

PlayerBase::PlayerBase(enums::CharacterType type, size_t id) : Entity(id, enums::TargetType::PLAYER), type(type)
{
}

void PlayerBase::Update(state::App &state, float elapsed_time)
{
    Entity::Update(state, elapsed_time);
}

void PlayerBase::MakeNextPlan(state::App &state, float elapsed_time)
{
    auto player = state.game_world.player_entity;

    auto enemy = GetCurrentTarget(
        state,
        PlayerBase::FilterByDistance(player->ref->position, 250.f),
        PlayerBase::OrderByDistanceAsc(player->ref->position)
    );

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

    auto attack_range = 60.0f;
    auto difference = enemy->ref->position - player->ref->position;
    auto distance = difference.mag();

    if (distance > attack_range)
    {
        // move towards the enemy
        auto target = player->ref->position + difference.norm() * std::min(25.0f, distance);
        plan = std::make_unique<plan::Move>(core::Vector(target), 45.f);
        // after walking add a second of delay
        plan_cooldown = 0.0f;
        return;
    }

    const auto *attack_name = "attack1";
    auto duration = 0.66f;
    auto hit_time_window =
        animator.GetFrameWindowTime(attack_name, 3, 3) / animator.GetAnimationSpeedForDuration(attack_name, duration);
    plan = std::make_unique<plan::DirectAttack>(plan::DirectAttackConfig{
        .target = enemy,
        .animation_name = attack_name,
        .hit_time_window = hit_time_window,
        .duration = duration,
        .max_range = attack_range * 1.5f,
        .damage = 40.0f,
    });
    SetPlan(state, std::move(plan), 1.35f);

    last_attack_target = std::move(enemy);
}

std::shared_ptr<EntityHandle>
PlayerBase::FindClosestEnemyTo(const state::App &state, const core::Vector &position) const
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

std::vector<std::pair<float, std::shared_ptr<EntityHandle>>> PlayerBase::GetRankedEnemies(
    const state::App &state,
    const std::function<bool(const Entity &)> &filter_by,
    const std::function<float(const Entity &)> &order_by
) const
{
    auto ranked_entities = std::vector<std::pair<float, std::shared_ptr<EntityHandle>>>();
    ranked_entities.reserve(state.game_world.entities.size());

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
        if (!filter_by(*entity->ref))
        {
            // ignore by custom filter
            continue;
        }

        const auto ranking = order_by(*entity->ref);
        const auto pair = std::make_pair(ranking, entity);
        ranked_entities.push_back(pair);
    }

    std::ranges::sort(
        ranked_entities,
        [](const auto &first, const auto &second) -> bool { return first.first < second.first; }
    );
    return ranked_entities;
}

std::shared_ptr<entity::EntityHandle> PlayerBase::GetCurrentTarget(
    const state::App &state,
    const std::function<bool(const Entity &)> &filter_by,
    const std::function<float(const Entity &)> &order_by
) const
{
    if (last_attack_target && last_attack_target->ref)
    {
        if (!last_attack_target->ref->is_removed && !last_attack_target->ref->is_dying)
        {
            if (filter_by(*last_attack_target->ref))
            {
                // still valid target
                return last_attack_target;
            }
        }
    }

    auto ranked_enemies = GetRankedEnemies(state, filter_by, order_by);
    if (ranked_enemies.empty())
    {
        return nullptr;
    }

    return ranked_enemies.at(0).second;
}

std::function<bool(const Entity &)> PlayerBase::FilterByDistance(const core::Vector &center, float max_distance)
{
    return [&center, max_distance](const auto &entity) { return (center - entity.position).mag() < max_distance; };
}

std::function<float(const Entity &)> PlayerBase::OrderByDistanceAsc(const core::Vector &center)
{
    return [&center](const auto &entity) { return (center - entity.position).mag(); };
}