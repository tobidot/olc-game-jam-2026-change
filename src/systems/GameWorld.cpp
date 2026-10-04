#include "systems/GameWorld.hpp"

#include "core/AssetManager.hpp"
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
#include "enums/Enums.hpp"
#include "exceptions/Exceptions.hpp"
#include "services/EntityService.hpp"
#include "services/RootService.hpp"
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
    // add scheduled entites
    for (auto &new_entity : state.game_world.new_entities)
    {
        state.game_world.entities.push_back(new_entity);
    }
    state.game_world.new_entities.clear();
    // Handle all entities
    for (auto &entity : state.game_world.entities)
    {
        entity->ref->Update(state, elapsed_time);
        HandleWorldBounds(state, *entity->ref, elapsed_time);
    }
    auto old_level_progress = state.game_world.player.level_progress;
    auto progress_steps = 25.0f;
    auto current_entity_level_progress =
        floorf(state.game_world.player_entity->ref->position.x / progress_steps) * progress_steps;
    auto level_progress_diff = current_entity_level_progress - old_level_progress;
    state.game_world.player.switch_cooldown = std::max(0.f, state.game_world.player.switch_cooldown - elapsed_time);

    // input based handling
    if (input.switchCharacter && state.game_world.player.switch_cooldown <= 0.0f)
    {
        auto type = std::array<enums::CharacterType, 4>({
            enums::CharacterType::WIZARD,
            enums::CharacterType::KNIGHT,
            enums::CharacterType::SAMURAI,
            enums::CharacterType::SHINOBI,
        });
        SwitchPlayerCharacterTo(assets, state, type.at(rand() % 4));
        state.game_world.player.switch_cooldown = 10.0f;
    }
    if (input.cheatSpawnEnemy)
    {
        auto type = input.cheatSpawnEnemyType;
        SpawnEnemy(assets, state, type);
    }

    //

    if (level_progress_diff > state.settings.screen_size.x * 0.4f)
    {
        state.game_world.player.level_progress +=
            floorf(std::max(20.0f, level_progress_diff / 5.0f) / 10.0f) * 10.0f * elapsed_time;
    }
    else if (level_progress_diff > state.settings.screen_size.x * 0.15f)
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

    auto old_level_spawn_rate_time = state.game_world.level.spawn_rate_time;
    auto new_level_spawn_rate_time = (state.game_world.level.spawn_rate_time += elapsed_time);
    for (const auto &spawn_item_entry : state.game_world.level.spawn_rate.spawns)
    {
        if (old_level_spawn_rate_time < spawn_item_entry.first && new_level_spawn_rate_time > spawn_item_entry.first)
        {
            const auto type = spawn_item_entry.second;
            SpawnEnemy(assets, state, type);
        }
    }
    state.game_world.level.spawn_rate_time =
        std::fmod(state.game_world.level.spawn_rate_time, state.game_world.level.spawn_rate.total_time);

    auto entities_to_delete = std::vector<std::vector<std::shared_ptr<entity::EntityHandle>>::iterator>();
    for (auto iterator = state.game_world.entities.begin(); iterator != state.game_world.entities.end(); iterator++)
    {
        if ((iterator->get()->ref)->is_removed)
        {
            entities_to_delete.push_back(iterator);
        }
    }
    for (const auto &entity : entities_to_delete)
    {
        state.game_world.entities.erase(entity);
    }
}

std::shared_ptr<entity::EntityHandle>
GameWorld::SwitchPlayerCharacterTo(const core::AssetManager &assets, state::App &state, enums::CharacterType type)
{
    entity::EntityRef new_character = nullptr;
    auto next_id = state.game_world.GetNextEntityID();

    switch (type)
    {
        case enums::CharacterType::KNIGHT:
        {
            new_character = std::make_shared<entity::PlayerKnight>(assets, state, next_id);
            break;
        }
        case enums::CharacterType::WIZARD:
        {
            new_character = std::make_shared<entity::PlayerWizard>(assets, state, next_id);
            break;
        }
        case enums::CharacterType::SAMURAI:
        {
            new_character = std::make_shared<entity::PlayerSamurai>(assets, state, next_id);
            break;
        }
        case enums::CharacterType::SHINOBI:
        {
            new_character = std::make_shared<entity::PlayerShinobi>(assets, state, next_id);
            break;
        }
        default:
        {
            throw exceptions::logic::LogicException("Enemy Type Unkown");
        }
    }

    auto &old_player = state.game_world.player_entity;
    new_character->health = new_character->max_health * (old_player->ref->health / old_player->ref->max_health);
    new_character->position = old_player->ref->position;
    old_player->ref->is_removed = true;
    state.game_world.player_entity->ref = new_character;

    return state.game_world.player_entity;
}

std::shared_ptr<entity::EntityHandle>
GameWorld::SpawnEnemy(const core::AssetManager &assets, state::App &state, enums::EnemyType type)
{
    return service::root()->entities->SpawnEnemy(type);
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
    entity.position.y = std::max(bounds.top, entity.position.y);
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
    auto player_entity = std::make_shared<entity::PlayerWizard>(assets, state, state.game_world.GetNextEntityID());
    player_entity->position = {75.f, 125.f};
    state.game_world.player_entity = std::make_shared<entity::EntityHandle>(player_entity);
    state.game_world.entities.push_back(state.game_world.player_entity);

    auto &level = state.game_world.level = state::Level();
    level.events = {
        state::LevelEvent{
            .at_progress = 25.0f,
            .spawns =
                {
                    enums::EnemyType::SKELETON,
                    enums::EnemyType::SKELETON,
                },
            .new_spawn_rate =
                state::LevelSpawnRate{
                    .total_time = 4.0f,
                    .spawns =
                        {
                            std::pair{2.f, enums::EnemyType::SKELETON},
                        },
                },
        },
        state::LevelEvent{
            .at_progress = 200.0f,
            .spawns = {enums::EnemyType::SATYR},
            .new_spawn_rate =
                state::LevelSpawnRate{
                    .total_time = 10.0f,
                    .spawns =
                        {
                            std::pair{3.f, enums::EnemyType::SKELETON},
                            std::pair{8.f, enums::EnemyType::SATYR},
                            std::pair{8.f, enums::EnemyType::SATYR},
                        },
                },
        },
        state::LevelEvent{
            .at_progress = 400.0f,
            .spawns = {enums::EnemyType::MINOTAUR},
            .new_spawn_rate =
                state::LevelSpawnRate{
                    .total_time = 10.0f,
                    .spawns =
                        {
                            std::pair{2.f, enums::EnemyType::MINOTAUR},
                            std::pair{7.f, enums::EnemyType::MINOTAUR},
                        },
                },
        },
        state::LevelEvent{
            .at_progress = 500.0f,
            .spawns = {},
            .new_spawn_rate =
                state::LevelSpawnRate{
                    .total_time = 10.0f,
                    .spawns = {},
                },
        },
        state::LevelEvent{
            .at_progress = 600.0f,
            .spawns = {enums::EnemyType::VAMPIRE, enums::EnemyType::VAMPIRE, enums::EnemyType::VAMPIRE},
            .new_spawn_rate =
                state::LevelSpawnRate{
                    .total_time = 15.0f,
                    .spawns =
                        {
                            std::pair{10.f, enums::EnemyType::SKELETON},
                            std::pair{10.f, enums::EnemyType::VAMPIRE},
                        },
                },
        },
        state::LevelEvent{
            .at_progress = 800.0f,
            .spawns = {enums::EnemyType::WEREWOLF},
            .new_spawn_rate = state::LevelSpawnRate{
                .total_time = 5.0f,
                .spawns = {
                    std::pair{1.f, enums::EnemyType::WEREWOLF},
                },
            },
        },
    };
    level.spawn_rate = state::LevelSpawnRate();
}
