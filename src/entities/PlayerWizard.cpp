#include "entities/PlayerWizard.hpp"

#include "core/AssetManager.hpp"

using namespace entity;

PlayerWizard::PlayerWizard(const core::AssetManager &assets, const state::App &state)
    : PlayerBase(enums::CharacterType::WIZARD)
{
    health = max_health = 200.0f;
    animator = *assets.wizard_animator;
    scale = {1.f, 1.f};
    current_animation = "walk";
    current_animation_time = 0.0f;
}

void PlayerWizard::Update(state::App &state, float elapsed_time)
{
    PlayerBase::Update(state, elapsed_time);
}