#include "entities/EnemySatyr.hpp"

#include "helper.hpp"
#include "plans/AimingMissleAttack.hpp"
#include "plans/DirectAttack.hpp"
#include "plans/Move.hpp"
#include "services/RootService.hpp"

using namespace entity;

EnemySatyr::EnemySatyr(const core::AssetManager &assets, const state::App &state, size_t id) : EnemyBase(id)
{
    health = max_health = 55.0f;
    animator = *assets.satyr_animator;
    scale = {.75f, .75f};
    shape.size = {10.f, 10.f};
    current_animation = "idle";
    current_animation_time = 0.0f;
}

/**
 * Skeletons are medium and nothing special
 */
void EnemySatyr::MakeNextPlan(state::App &state, float elapsed_time)
{
    const auto attack_range = 300.0f;
    auto difference = state.game_world.player_entity->ref->position - position;
    auto distance = difference.mag();

    if (distance < attack_range)
    {
        float animation_duration = 1.6f;
        std::string attack_name;
        std::pair<size_t, size_t> cast_window = {0, 0};
        std::pair<size_t, size_t> sfx_window = {0, 0};
        switch (rand() % 1)
        {
            case 0:
            {
                attack_name = "attack1";
                cast_window = {7, 7};
                sfx_window = {2, 7};
                break;
            }
            default:
                throw std::runtime_error("Unknown attack pattern");
        }

        auto cast_frame_window = animator.GetFrameWindowTime(attack_name, cast_window.first, cast_window.second);
        auto sfx_frame_window = animator.GetFrameWindowTime(attack_name, sfx_window.first, sfx_window.second);
        auto animation_speed = animator.GetAnimationSpeedForDuration(attack_name, animation_duration);
        auto self = state.game_world.FindEntityHandle(id);

        plan = std::make_unique<plan::AimingMissleAttack>(plan::AimingMissleAttackConfig{
            .source = self,
            .target = state.game_world.player_entity,
            .sfx_cast = service::root()->assets->sfx_swarp,
            .animation_name = attack_name,
            .cast_time_window = cast_frame_window / animation_speed,
            .sfx_time_window = sfx_frame_window / animation_speed,
            .duration = animation_duration,
            .damage = 15.0f,
        });
        SetPlan(state, std::move(plan), 1.0f);
    }
    else
    {
        auto target = position + difference.norm() * std::min(75.0f, difference.mag());
        auto new_plan = std::make_unique<plan::Move>(core::Vector(target), 35.f);
        SetPlan(state, std::move(new_plan), .0f);
    }
}