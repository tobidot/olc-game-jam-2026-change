#include "systems/GameWorld.hpp"

#include "core/AssetManager.hpp"
#include "entities/EnemyGhost.hpp"
#include "entities/EnemyMinotaur.hpp"
#include "entities/EnemySatyr.hpp"
#include "entities/EnemySkeleton.hpp"
#include "entities/EnemyWerewolf.hpp"
#include "entities/Entity.hpp"
#include "entities/PlayerKnight.hpp"
#include "enums/Enums.hpp"
#include "exceptions/Exceptions.hpp"
#include "state/Level.hpp"
#include "systems/GameInput.hpp"

#include <cassert>
#include <memory>

using namespace systems;

void GameWorld::Seed(int generator_seed, const core::AssetManager &asset_manager)
{
}

void GameWorld::Update(const core::AssetManager &assets, const GameInput &input, state::App &state, float elapsed_time)
{
    for (auto &entity : state.game_world.entities)
    {
        entity->Update(state, elapsed_time);
        HandleWorldBounds(state, *entity, elapsed_time);
    }
    auto old_level_progress = state.game_world.player.level_progress;
    auto current_entity_level_progress = floorf(state.game_world.player_entity->position.x / 50.0f) * 50.0f;
    auto level_progress_diff = current_entity_level_progress - old_level_progress;

    if (level_progress_diff > state.settings.screen_size.x * 0.6f)
    {
        state.game_world.player.level_progress +=
            floorf(std::max(20.0f, level_progress_diff / 5.0f) / 10.0f) * 10.0f * elapsed_time;
    }
    else if (level_progress_diff > state.settings.screen_size.x * 0.3f)
    {
        state.game_world.player.level_progress += 20.0f * elapsed_time;
    }
    auto new_level_progress = state.game_world.player.level_progress;

    // figure out which events are triggered by the level progress increase
    for (const auto &event : state.game_world.level.events)
    {
        if (old_level_progress < event.at_progress && new_level_progress >= event.at_progress)
        {
            state.game_world.level.spawn_rate = event.new_spawn_rate;
            state.game_world.level.spawn_rate_time = 0.0f;
            for (auto type : event.spawns)
            {
                SpawnEnemy(assets, state, type);
            }
        }
    }
}

std::shared_ptr<entity::Entity>
GameWorld::SpawnEnemy(const core::AssetManager &assets, state::App &state, enums::EnemyType type)
{
    // spawn player
    std::shared_ptr<entity::Entity> entity;

    switch (type)
    {
        case enums::EnemyType::GHOST:
        {
            entity = std::make_shared<entity::EnemyGhost>(assets, state);
            break;
        }
        case enums::EnemyType::MINOTAUR:
        {
            entity = std::make_shared<entity::EnemyMinotaur>(assets, state);
            break;
        }
        case enums::EnemyType::SATYR:
        {
            entity = std::make_shared<entity::EnemySatyr>(assets, state);
            break;
        }
        case enums::EnemyType::SKELETON:
        {
            entity = std::make_shared<entity::EnemySkeleton>(assets, state);
            break;
        }
        case enums::EnemyType::WEREWOLF:
        {
            entity = std::make_shared<entity::EnemyWerewolf>(assets, state);
            break;
        }
        default:
        {
            throw exceptions::logic::LogicException("Enemy Type Unkown");
        }
    }

    // spawn at the right border of the screen
    auto spawn_x = state.game_world.player.level_progress + state.settings.screen_size.x;
    auto random = (static_cast<float>(rand()) / static_cast<float>(std::numeric_limits<int>::max()));
    auto spawn_y = state.settings.screen_size.y * random;
    entity->position = {spawn_x, spawn_y};

    state.game_world.entities.push_back(entity);

    return entity;
}

void GameWorld::HandleWorldBounds(state::App &state, entity::Entity &entity, float elapsed_time)
{
    auto bounds = state.game_world.boundaries;
    if (bounds.Contains(entity.position))
    {
        // all good
        return;
    }

    entity.position.x = std::max(bounds.left, entity.position.x);
    entity.position.x = std::min(bounds.right, entity.position.x);
    entity.position.y = std::max(bounds.left, entity.position.y);
    entity.position.y = std::min(bounds.bottom, entity.position.y);
}

void GameWorld::Load(const core::AssetManager &assets, state::App &state)
{
    state.game_world.render_offset_top = 100.0f;
    state.game_world.boundaries = {
        .top = 0,
        .left = 0,
        .bottom = 200.f,
        .right = 1000.f,
    };

    // spawn player
    auto player_entity = std::make_shared<entity::PlayerKnight>(assets, state);
    player_entity->position = {0.f, 125.f};
    state.game_world.player_entity = player_entity;
    state.game_world.entities.push_back(player_entity);

    auto &level = state.game_world.level = state::Level();
    level.events = {
        state::LevelEvent{
            .at_progress = 1.0f,
            .spawns =
                {
                    enums::EnemyType::GHOST,
                    enums::EnemyType::MINOTAUR,
                    enums::EnemyType::SATYR,
                    enums::EnemyType::SKELETON,
                    enums::EnemyType::WEREWOLF,
                },
            .new_spawn_rate = state::LevelSpawnRate(),
        },
        state::LevelEvent{
            .at_progress = 400.0f,
            .spawns =
                {
                    enums::EnemyType::GHOST,
                },
            .new_spawn_rate = state::LevelSpawnRate(),
        },
    };
    level.spawn_rate = state::LevelSpawnRate();
}