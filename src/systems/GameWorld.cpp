#include "systems/GameWorld.hpp"

#include "core/AssetManager.hpp"
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

void GameWorld::Load(const core::AssetManager &assets, state::App &state)
{
    auto player_entity = std::make_shared<entity::PlayerKnight>(assets, state);
    player_entity->position = {0.f, 125.f};
    state.game_world.player_entity = player_entity;
    state.game_world.entities.push_back(player_entity);
}