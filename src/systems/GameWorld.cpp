#include "systems/GameWorld.hpp"

#include "core/AssetManager.hpp"
#include "entities/EnemyGhost.hpp"
#include "entities/Entity.hpp"
#include "entities/PlayerKnight.hpp"
#include "systems/GameInput.hpp"

#include <cassert>
#include <memory>

using namespace systems;

void GameWorld::Seed(int generator_seed, const core::AssetManager &asset_manager)
{
}

void GameWorld::Update(const GameInput &input, state::App &state, float elapsed_time)
{
    for (auto &entity : state.game_world.entities)
    {
        entity->Update(state, elapsed_time);
        HandleWorldBounds(state, *entity, elapsed_time);
    }
    auto current_entity_level_progress = floorf(state.game_world.player_entity->position.x / 50.0f) * 50.0f;
    auto level_progress_diff = current_entity_level_progress - state.game_world.player.level_progress;

    if (level_progress_diff > state.settings.screen_size.x * 0.5f)
    {
        state.game_world.player.level_progress +=
            floorf(std::max(20.0f, level_progress_diff / 5.0f) / 10.0f) * 10.0f * elapsed_time;
    }
    else if (level_progress_diff > state.settings.screen_size.x * 0.15f)
    {
        state.game_world.player.level_progress += 20.0f * elapsed_time;
    }
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

    // spawn enemy
    auto enemy_entity = std::make_shared<entity::EnemyGhost>(assets, state);
    enemy_entity->position = {250.f, 125.f};
    state.game_world.entities.push_back(enemy_entity);
}