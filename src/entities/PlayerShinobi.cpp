#include "entities/PlayerShinobi.hpp"

#include "core/AssetManager.hpp"
#include "exceptions/Exceptions.hpp"
#include "helper.hpp"
#include "plans/DirectAttack.hpp"
#include "plans/Move.hpp"
#include "plans/Teleport.hpp"

using namespace entity;

PlayerShinobi::PlayerShinobi(const core::AssetManager &assets, const state::App &state, size_t id)
    : PlayerBase(enums::CharacterType::SHINOBI, id)
{
    health = max_health = 200.0f;
    armor = 1.0f;
    animator = *assets.shinobi_animator;
    scale = {.8f, .8f};
    current_animation = "walk";
    current_animation_time = 0.0f;
}

void PlayerShinobi::Update(state::App &state, float elapsed_time)
{
    PlayerBase::Update(state, elapsed_time);
}

void PlayerShinobi::MakeNextPlan(state::App &state, float elapsed_time)
{
    auto player = state.game_world.player_entity;
    // chose enemies with the lowest health
    auto enemy = GetCurrentTarget(
        state,
        PlayerBase::FilterByDistance(position, 200.f),
        [](const auto &entity) { return entity.health; }
    );
    auto movement_speed = 60.0f;

    if (!enemy)
    {
        // without target move right
        auto target = player->ref->position + core::Vector{movement_speed, 0.f};
        plan = std::make_unique<plan::Move>(core::Vector(target), movement_speed);
        SetPlan(state, std::move(plan), 0.0f);
        return;
    }

    auto duration = 0.45f;
    auto attack_range = 35.0f;
    auto damage = 25.0f;
    auto cooldown = 0.25f;
    auto difference = enemy->ref->position - player->ref->position;
    auto distance = difference.mag();
    auto min_teleport_range = 60.0f;
    auto max_teleport_range = 175.0f;

    if (distance > attack_range)
    {

        if (distance > min_teleport_range && distance < max_teleport_range)
        {
            std::cout << "tele " << distance << "\n";
            // teleport in a certain distance
            const auto *animation_name = "hurt";
            const auto duration = 0.2f;
            const auto animation_speed = animator.GetAnimationSpeedForDuration(animation_name, duration);
            const auto effect_time_window = animator.GetFrameWindowTime(animation_name, 0, 0) / animation_speed;
            plan = std::make_unique<plan::Teleport>(plan::TeleportConfig{
                .animation_name = animation_name,
                .target = enemy,
                .effect_time_window = effect_time_window,
                .duration = duration,
            });
            SetPlan(state, std::move(plan), 0.1f);
            // after tp this is my target
            last_attack_target = std::move(enemy);
            return;
        }

        // move towards the enemy when to far away or too close for teleport
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
            hit_frame = 4;
            break;
        }
        case 1:
        {
            attack_name = "attack2";
            hit_frame = 1;
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