#include "entities/EnemyWerewolf.hpp"

#include "helper.hpp"
#include "plans/BasePlan.hpp"
#include "plans/Chase.hpp"
#include "plans/DirectAttack.hpp"

using namespace entity;

EnemyWerewolf::EnemyWerewolf(const core::AssetManager &assets, const state::App &state, size_t id) : EnemyBase(id)
{
    health = max_health = 125.0f;
    animator = *assets.werewolf_animator;
    scale = {.9f, .9f};
    shape.size = {17.f, 17.f};
    current_animation = "idle";
    current_animation_time = 0.0f;
}

/**
 * Werewolf dangerous fast damage
 */
void EnemyWerewolf::MakeNextPlan(state::App &state, float elapsed_time)
{
    const auto attack_range = 45.0f;
    const auto chase_range = 175.0f;
    const auto &player = state.game_world.player_entity;
    auto difference = player->ref->position - position;
    auto distance = difference.mag();

    if (distance < attack_range)
    {
        auto duration = 0.45f;
        std::string attack_name;
        size_t attack_frame = 0;
        switch (rand() % 3)
        {
            case 0:
            {
                attack_name = "attack1";
                attack_frame = 4;
                break;
            }
            case 1:
            {
                attack_name = "attack2";
                attack_frame = 3;
                break;
            }
            case 2:
            {
                attack_name = "attack3";
                attack_frame = 3;
                break;
            }
            default:
                throw std::runtime_error("Unknown attack pattern");
        }

        auto self = state.game_world.FindEntityHandle(id);
        auto hit_time_window = animator.GetFrameWindowTime(attack_name, attack_frame, attack_frame) /
                               animator.GetAnimationSpeedForDuration(attack_name, duration);
        plan = std::make_unique<plan::DirectAttack>(plan::DirectAttackConfig{
            .target = player,
            .animation_name = attack_name,
            .hit_time_window = hit_time_window,
            .duration = duration,
            .max_range = attack_range * 1.5f,
            .damage = 6.0f,
        });
        SetPlan(state, std::move(plan), 0.f);
    }
    else if (distance < chase_range)
    {
        auto new_plan = std::make_unique<plan::Chase>(player, 95.0f);
        SetPlan(state, std::move(new_plan), .125f);
    }
    else
    {
        // sit idle
        SetNextAnimation("idle");

        // auto offset_x = static_cast<float>((rand() % 100) - 50);
        // auto offset_y = static_cast<float>((rand() % 100) - 50);
        // auto target = position + core::Vector{offset_x, offset_y};
        // auto new_plan = std::make_unique<plan::Move>(core::Vector(target), 35.f);
        // SetPlan(state, std::move(new_plan), .8f);
    }
}