#include "services/EntityService.hpp"

#include "core/AssetManager.hpp"
#include "entities/EffectFireBall.hpp"
#include "entities/EnemyGhost.hpp"
#include "entities/EnemyMinotaur.hpp"
#include "entities/EnemySatyr.hpp"
#include "entities/EnemySkeleton.hpp"
#include "entities/EnemyVampire.hpp"
#include "entities/EnemyWerewolf.hpp"
#include "entities/Entity.hpp"
#include "entities/PlayerKnight.hpp"
#include "entities/PlayerSamurai.hpp"
#include "entities/PlayerShinobi.hpp"
#include "entities/PlayerWizard.hpp"
#include "exceptions/Exceptions.hpp"

#include <memory>

using namespace service;

EntityService::EntityService(const core::AssetManager *assets, state::App *state) : assets(assets), state(state)
{
}

std::shared_ptr<entity::EntityHandle> EntityService::SpawnEnemy(enums::EnemyType type) const
{
    entity::EntityRef entity = nullptr;
    const auto next_id = state->game_world.GetNextEntityID();

    switch (type)
    {
        case enums::EnemyType::GHOST:
        {
            entity = std::make_shared<entity::EnemyGhost>(*assets, *state, next_id);
            break;
        }
        case enums::EnemyType::MINOTAUR:
        {
            entity = std::make_shared<entity::EnemyMinotaur>(*assets, *state, next_id);
            break;
        }
        case enums::EnemyType::SATYR:
        {
            entity = std::make_shared<entity::EnemySatyr>(*assets, *state, next_id);
            break;
        }
        case enums::EnemyType::SKELETON:
        {
            entity = std::make_shared<entity::EnemySkeleton>(*assets, *state, next_id);
            break;
        }
        case enums::EnemyType::WEREWOLF:
        {
            entity = std::make_shared<entity::EnemyWerewolf>(*assets, *state, next_id);
            break;
        }
        case enums::EnemyType::VAMPIRE:
        {
            entity = std::make_shared<entity::EnemyVampire>(*assets, *state, next_id);
            break;
        }
        default:
        {
            throw exceptions::logic::LogicException("Enemy Type Unkown");
        }
    }

    // spawn at the right border of the screen
    auto spawn_x = state->game_world.player.level_progress + state->settings.screen_size.x;
    auto random = (static_cast<float>(rand()) / static_cast<float>(std::numeric_limits<int>::max()));
    auto spawn_y = state->settings.screen_size.y * random;
    entity->position = {spawn_x, spawn_y};

    assets->sfx_select->Play();

    auto handle = std::make_shared<entity::EntityHandle>(entity);
    state->game_world.new_entities.push_back(handle);

    return handle;
}

std::shared_ptr<entity::EntityHandle>
EntityService::SpawnEffect(enums::EffectType type, const core::Vector &position) const
{
    entity::EntityRef entity = nullptr;
    const auto next_id = state->game_world.GetNextEntityID();

    switch (type)
    {
        case enums::EffectType::FIREBALL:
        {
            entity = std::make_shared<entity::EffectFireBall>(*assets, *state, next_id);
            break;
        }
        default:
        {
            throw exceptions::logic::LogicException("Effect Type Unkown");
        }
    }

    entity->position = position;
    assets->sfx_select->Play();

    auto handle = std::make_shared<entity::EntityHandle>(entity);
    state->game_world.new_entities.push_back(handle);

    return handle;
}

std::vector<std::shared_ptr<entity::EntityHandle>> EntityService::Pick(
    core::Vector center, float distance, const std::vector<enums::TargetType> &target_types

) const
{
    auto buffer = std::vector<std::shared_ptr<entity::EntityHandle>>();
    for (const auto &entity : state->game_world.entities)
    {
        auto target_type = entity->ref->target_type;
        auto is_all_valid_target = target_types.empty();
        auto is_valid_target = std::ranges::find_if(
                                   target_types,
                                   [target_type](const enums::TargetType &current_target_type)
                                   { return current_target_type == target_type; }
                               ) != target_types.end();
        if (is_all_valid_target || is_valid_target)
        {
            auto current_distance = (entity->ref->position - center).mag();
            auto entity_radius = entity->ref->shape.size.mag() / 2.f;
            if (current_distance <= distance + entity_radius)
            {
                buffer.push_back(entity);
            }
        }
    }
    return buffer;
}