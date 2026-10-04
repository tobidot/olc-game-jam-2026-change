#include "entities/PlayerSamurai.hpp"

#include "core/AssetManager.hpp"
#include "helper.hpp"
#include "plans/AreaAttack.hpp"
#include "plans/BasePlan.hpp"
#include "plans/Move.hpp"
#include "services/RootService.hpp"

using namespace entity;

PlayerSamurai::PlayerSamurai(const core::AssetManager &assets, const state::App &state, size_t id)
    : PlayerBase(enums::CharacterType::SAMURAI, id)
{
    health = max_health = 250.0f;
    armor = 3.0f;
    animator = *assets.samurai_animator;
    scale = {1.2f, 1.2f};
    shape.size = {18.f, 18.f};
    current_animation = "idle";
    current_animation_time = 0.0f;
}

void PlayerSamurai::Update(state::App &state, float elapsed_time)
{
    PlayerBase::Update(state, elapsed_time);
}

/**
 * AOE damage dealer
 */
void PlayerSamurai::MakeNextPlan(state::App &state, float elapsed_time)
{
    auto player = state.game_world.player_entity;
    auto enemy = GetCurrentTarget(
        state,
        PlayerBase::FilterByDistance(player->ref->position, 160.f),
        PlayerBase::OrderByDistanceAsc(player->ref->position)
    );
    auto movement_speed = 30.0f;

    if (!enemy)
    {
        // without target move right
        auto target = player->ref->position + core::Vector{movement_speed, 0.f};
        plan = std::make_unique<plan::Move>(core::Vector(target), movement_speed);
        SetPlan(state, std::move(plan), 0.0f);
        return;
    }

    auto duration = 1.2f;
    auto cooldown = 1.5f;
    auto attack_range = 60.0f;
    auto area = 45.0f;
    auto damage = 35.0f;
    auto difference = enemy->ref->position - player->ref->position;
    auto distance = difference.mag();
    // target the attack in direction of the enemy at the area radius distance
    auto target = core::Vector(position + difference.norm() * area);

    if (distance > attack_range)
    {
        // move towards the enemy when to far away
        auto target = player->ref->position + difference.norm() * std::min(movement_speed, distance);
        plan = std::make_unique<plan::Move>(core::Vector(target), movement_speed);
        SetPlan(state, std::move(plan), 0.0f);
        return;
    }

    const auto attack_index = rand() % 3;
    std::string attack_name;
    auto hit_frame = 0;
    switch (attack_index)
    {
        case 0:
        {
            attack_name = "attack1";
            hit_frame = 2;
            break;
        }
        case 1:
        {
            attack_name = "attack2";
            hit_frame = 4;
            break;
        }
        case 2:
        {
            attack_name = "attack3";
            hit_frame = 2;
            break;
        }
        default:
            throw std::runtime_error("Unknown attack pattern");
    };

    auto animation_speed = animator.GetAnimationSpeedForDuration(attack_name, duration);
    auto hit_time_window = animator.GetFrameWindowTime(attack_name, hit_frame, hit_frame) / animation_speed;
    plan = std::make_unique<plan::AreaAttack>(plan::AreaAttackConfig{
        .source = player,
        .sfx_hit = service::root()->assets->sfx_samurai_attack,
        .animation_name = attack_name,
        .hit_time_window = hit_time_window,
        .duration = duration,
        .target = target,
        .targets = {enums::TargetType::ENEMY},
        .area = area,
        .damage = damage,
    });

    // mark enemy as current target and attack
    last_attack_target = std::move(enemy);
    SetPlan(state, std::move(plan), cooldown);
}