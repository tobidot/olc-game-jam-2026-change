#include "systems/GameWorld.hpp"

#include "core/AssetManager.hpp"
#include "entities/EnemyGhost.hpp"
#include "entities/EnemyMinotaur.hpp"
#include "entities/EnemySatyr.hpp"
#include "entities/EnemySkeleton.hpp"
#include "entities/EnemyVampire.hpp"
#include "entities/EnemyWerewolf.hpp"
#include "entities/Entity.hpp"
#include "entities/PlayerBase.hpp"
#include "entities/PlayerKnight.hpp"
#include "entities/PlayerSamurai.hpp"
#include "entities/PlayerShinobi.hpp"
#include "entities/PlayerWizard.hpp"
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
    auto current_entity_level_progress = floorf((*state.game_world.player_entity)->position.x / 50.0f) * 50.0f;
    auto level_progress_diff = current_entity_level_progress - old_level_progress;

    if (input.switchCharacter)
    {
        auto type = std::array<enums::CharacterType, 4>({
            enums::CharacterType::WIZARD,
            enums::CharacterType::KNIGHT,
            enums::CharacterType::SAMURAI,
            enums::CharacterType::SHINOBI,
        });
        SwitchPlayerCharacterTo(assets, state, type.at(rand() % 4));
    }

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

    auto entities_to_delete = std::vector<std::vector<std::shared_ptr<entity::Entity>>::iterator>();
    for (auto iterator = state.game_world.entities.begin(); iterator != state.game_world.entities.end(); iterator++)
    {
        if (iterator->get()->is_removed)
        {
            entities_to_delete.push_back(iterator);
        }
    }
    for (const auto &entity : entities_to_delete)
    {
        state.game_world.entities.erase(entity);
    }
}

std::shared_ptr<entity::Entity>
GameWorld::SwitchPlayerCharacterTo(const core::AssetManager &assets, state::App &state, enums::CharacterType type)
{
    std::shared_ptr<entity::Entity> new_character = nullptr;

    std::cout << "switching " << (int)type << "\n";

    switch (type)
    {
        case enums::CharacterType::KNIGHT:
        {
            new_character = std::make_shared<entity::PlayerKnight>(assets, state);
            break;
        }
        case enums::CharacterType::WIZARD:
        {
            new_character = std::make_shared<entity::PlayerWizard>(assets, state);
            break;
        }
        case enums::CharacterType::SAMURAI:
        {
            new_character = std::make_shared<entity::PlayerSamurai>(assets, state);
            break;
        }
        case enums::CharacterType::SHINOBI:
        {
            new_character = std::make_shared<entity::PlayerShinobi>(assets, state);
            break;
        }
        default:
        {
            throw exceptions::logic::LogicException("Enemy Type Unkown");
        }
    }

    auto &old_player = **state.game_world.player_entity;
    new_character->health = new_character->max_health * (old_player.health / old_player.max_health);
    new_character->position = old_player.position;
    old_player.is_removed = true;
    *state.game_world.player_entity = new_character;

    state.game_world.entities.push_back(new_character);

    return new_character;
}

std::shared_ptr<entity::Entity>
GameWorld::SpawnEnemy(const core::AssetManager &assets, state::App &state, enums::EnemyType type)
{
    // spawn player
    std::shared_ptr<entity::Entity> entity = nullptr;

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
        case enums::EnemyType::VAMPIRE:
        {
            entity = std::make_shared<entity::EnemyVampire>(assets, state);
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

    assets.sfx_select->Play();

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
    state.game_world.player_entity = std::make_shared<std::shared_ptr<entity::Entity>>(player_entity);
    player_entity->position = {75.f, 125.f};
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
                    enums::EnemyType::VAMPIRE,
                },
            .new_spawn_rate =
                state::LevelSpawnRate{
                    .total_time = 10.0f,
                    .spawns =
                        {
                            std::pair{1.f, enums::EnemyType::SKELETON},
                            std::pair{1.f, enums::EnemyType::SKELETON},
                            std::pair{3.f, enums::EnemyType::SKELETON},
                            std::pair{4.f, enums::EnemyType::GHOST},
                            std::pair{6.f, enums::EnemyType::VAMPIRE},
                            std::pair{7.5f, enums::EnemyType::GHOST},
                        },
                },
        },
        state::LevelEvent{
            .at_progress = 400.0f,
            .spawns = {enums::EnemyType::SATYR},
            .new_spawn_rate = state::LevelSpawnRate{
                .total_time = 15.0f,
                .spawns = {
                    std::pair{5.f, enums::EnemyType::WEREWOLF},
                    std::pair{10.f, enums::EnemyType::MINOTAUR},
                    std::pair{15.f, enums::EnemyType::SATYR},
                },
            },
        },
    };
    level.spawn_rate = state::LevelSpawnRate();
}