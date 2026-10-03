#include "entities/EnemyGhost.hpp"

#include "plans/BasePlan.hpp"
#include "plans/DirectAttack.hpp"
#include "plans/Move.hpp"

using namespace entity;

EnemyGhost::EnemyGhost(const core::AssetManager &assets, const state::App &state, size_t id) : EnemyBase(id)
{
    health = max_health = 25.0f;
    animator = *assets.ghost_animator;
    shape.size = {10.f, 10.f};
    scale = {.45f, .45f};
    z_offset = 15.0f;
    current_animation = "walk";
    current_animation_time = 0.0f;
}

/**
 * Ghost Moves fast and
 */
void EnemyGhost::MakeNextPlan(state::App &state, float elapsed_time)
{
    const auto attack_range = 25.0f;
    auto difference = state.game_world.player_entity->ref->position - position;
    auto distance = difference.mag();

    if (distance < attack_range)
    {
        auto self = state.game_world.FindEntityHandle(id);
        plan = std::make_unique<plan::DirectAttack>(plan::DirectAttack::FromAnimationFrame(
            plan::DirectAttackFromAnimationFrameConfig{
                .source = self,
                .target = state.game_world.player_entity,
                .animation_name = "attack1",
                .hit_frame = 4,
                .duration = 0.5f,
                .max_range = attack_range * 1.5f,
                .damage = 7.0f,
            }
        ));
        // after attackin add a second of delay
        SetPlan(state, std::move(plan), 0.5f);
    }
    else
    {
        auto target = position + difference.norm() * std::min(100.0f, difference.mag());
        auto new_plan = std::make_unique<plan::Move>(core::Vector(target), 50.f);
        SetPlan(state, std::move(new_plan), .25f);
    }
}