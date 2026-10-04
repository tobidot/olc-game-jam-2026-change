#include "entities/PlayerWizard.hpp"

#include "core/AssetManager.hpp"
#include "helper.hpp"
#include "plans/BasePlan.hpp"
#include "plans/ChainPlan.hpp"
#include "plans/Channel.hpp"
#include "plans/Chase.hpp"
#include "plans/Die.hpp"
#include "plans/DirectAttack.hpp"
#include "plans/Explode.hpp"
#include "plans/Move.hpp"
#include "services/EntityService.hpp"
#include "services/RootService.hpp"

using namespace entity;

PlayerWizard::PlayerWizard(const core::AssetManager &assets, const state::App &state, size_t id)
    : PlayerBase(enums::CharacterType::WIZARD, id)
{
    health = max_health = 80.0f;
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
        PlayerBase::FilterByDistance(player->ref->position, 325.f),
        PlayerBase::OrderByDistanceAsc(player->ref->position)
    );
    auto movement_speed = 35.0f;

    if (!enemy)
    {
        // without target move right
        auto target = player->ref->position + core::Vector{movement_speed, 0.f};
        plan = std::make_unique<plan::Move>(core::Vector(target), movement_speed);
        SetPlan(state, std::move(plan), 0.0f);
        return;
    }

    const auto *attack_name = "cast1";
    auto duration = 2.55f;
    auto attack_range = 225.0f;
    auto damage = 60.0f;
    auto cooldown = 0.65f;
    auto difference = enemy->ref->position - player->ref->position;
    auto distance = difference.mag();
    auto animation_speed = animator.GetAnimationSpeedForDuration(attack_name, duration);
    auto attack_frame = 12;
    auto cast_time_window = animator.GetFrameWindowTime(attack_name, attack_frame, attack_frame) / animation_speed;
    auto sfx_time_window = animator.GetFrameWindowTime(attack_name, 1, attack_frame) / animation_speed;

    if (distance > attack_range)
    {
        // move towards the enemy when to far away
        auto target = player->ref->position + difference.norm() * std::min(movement_speed, distance);
        plan = std::make_unique<plan::Move>(core::Vector(target), movement_speed);
        SetPlan(state, std::move(plan), 0.0f);
        return;
    }

    auto on_cast = [this] { this->OnCast(); };
    plan = std::make_unique<plan::Channel>(plan::ChannelConfig{
        .source = player,
        .target = enemy,
        .sfx_cast = service::root()->assets->sfx_swarp,
        .animation_name = attack_name,
        .cast_time_window = cast_time_window,
        .sfx_time_window = sfx_time_window,
        .duration = duration,
        .on_cast = on_cast,
    });

    // mark enemy as current target and attack
    last_attack_target = std::move(enemy);
    SetPlan(state, std::move(plan), cooldown);
}

void PlayerWizard::OnCast()
{
    auto *state = service::root()->state;
    const auto attack_range = 250.0f;
    auto &enemy = last_attack_target;
    auto &self = state->game_world.player_entity;

    if (!last_attack_target)
    {
        return;
    }

    auto duration = 0.45f;
    size_t attack_frame = 5;

    auto spawn_pixel_offset = GetCurrentAnchorPixelOffset("weapon");
    auto spawn_position = core::Vector(position + core::Vector{spawn_pixel_offset.x, 0.f});
    auto effect = service::root()->entities->SpawnEffect(enums::EffectType::PIERCING_LIGHT, spawn_position);
    effect->ref->z_offset = spawn_pixel_offset.y;

    auto target = core::Vector(position + (enemy->ref->position - position).norm() * 250.f);
    auto move_plan = std::make_unique<plan::Move>(target, 60.0f);

    std::string damage_animation_name = "idle";
    float damage_duration = 0.1f;
    float damage_animation_speed =
        effect->ref->animator.GetAnimationSpeedForDuration(damage_animation_name, damage_duration);
    std::pair<float, float> damage_time_window =
        effect->ref->animator.GetFrameWindowTime(damage_animation_name, 0, 0) / damage_animation_speed;
    auto damage_plan = std::make_unique<plan::DirectAttack>(plan::DirectAttackConfig{
        .target = enemy,
        .animation_name = damage_animation_name,
        .hit_time_window = damage_time_window,
        .duration = damage_duration,
        .max_range = 100.0f,
        .damage = 10.f,
    });

    auto die_plan = std::make_unique<plan::Die>(plan::DieConfig{
        .animation_name = "die",
        .duration = 0.33f,
    });
    std::vector<std::unique_ptr<plan::BasePlan>> plans = std::vector<std::unique_ptr<plan::BasePlan>>();
    plans.reserve(3);
    plans.push_back(std::move(move_plan));
    plans.push_back(std::move(damage_plan));
    plans.push_back(std::move(die_plan));
    auto chain_plan = std::make_unique<plan::ChainPlan>(std::move(plans));
    effect->ref->SetPlan(*state, std::move(chain_plan));
}