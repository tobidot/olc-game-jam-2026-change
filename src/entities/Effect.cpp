#include "entities/Effect.hpp"

using namespace entity;

Effect::Effect(const core::AssetManager &assets, const state::App &state, size_t id)
    : Entity(id, enums::TargetType::EFFECT)
{
}

void Effect::Update(state::App &state, float elapsed_time)
{
    Entity::Update(state, elapsed_time);
}