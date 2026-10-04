#include "entities/PlayerWizard.hpp"

#include "core/AssetManager.hpp"
#include "helper.hpp"
#include "plans/BasePlan.hpp"
#include "plans/DirectAttack.hpp"
#include "plans/Move.hpp"

using namespace entity;

PlayerWizard::PlayerWizard(const core::AssetManager &assets, const state::App &state, size_t id)
    : PlayerBase(enums::CharacterType::WIZARD, id)
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

void PlayerWizard::MakeNextPlan(state::App &state, float elapsed_time)
{
    auto player = state.game_world.player_entity;
    auto enemy = GetCurrentTarget(
        state,
        PlayerBase::FilterByDistance(player->ref->position, 250.f),
        PlayerBase::OrderByDistanceAsc(player->ref->position)
    );
    auto movement_speed = 22.0f;

    if (!enemy)
    {
        // without target move right
        auto target = player->ref->position + core::Vector{movement_speed, 0.f};
        plan = std::make_unique<plan::Move>(core::Vector(target), movement_speed);
        SetPlan(state, std::move(plan), 0.0f);
        return;
    }

    const auto *attack_name = "attack1";
    auto duration = 0.66f;
    auto attack_range = 60.0f;
    auto damage = 40.0f;
    auto cooldown = 1.65f;
    auto difference = enemy->ref->position - player->ref->position;
    auto distance = difference.mag();
    auto animation_speed = animator.GetAnimationSpeedForDuration(attack_name, duration);
    auto hit_time_window = animator.GetFrameWindowTime(attack_name, 3, 3) / animation_speed;

    if (distance > attack_range)
    {
        // move towards the enemy when to far away
        auto target = player->ref->position + difference.norm() * std::min(movement_speed, distance);
        plan = std::make_unique<plan::Move>(core::Vector(target), movement_speed);
        SetPlan(state, std::move(plan), 0.0f);
        return;
    }

    plan = std::make_unique<plan::DirectAttack>(plan::DirectAttackConfig{
        .target = enemy,
        .animation_name = attack_name,
        .hit_time_window = hit_time_window,
        .duration = duration,
        .max_range = attack_range * 1.5f,
        .damage = damage,
    });

    // mark enemy as current target and attack
    last_attack_target = std::move(enemy);
    SetPlan(state, std::move(plan), cooldown);
}