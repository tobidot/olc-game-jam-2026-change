#include "entities/EnemyMinotaur.hpp"

#include "plans/DirectAttack.hpp"
#include "plans/Move.hpp"

using namespace entity;

EnemyMinotaur::EnemyMinotaur(const core::AssetManager &assets, const state::App &state, size_t id) : EnemyBase(id)
{
    health = max_health = 225.0f;
    animator = *assets.minotaur_animator;
    shape.size = {25.f, 25.f};
    scale = {1.f, 1.f};
    current_animation = "walk";
    current_animation_time = 0.0f;
}

/**
 * Minotaur are heavy and regenerate
 */
void EnemyMinotaur::MakeNextPlan(state::App &state, float elapsed_time)
{
    const auto attack_range = 45.0f;
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
                .duration = 1.5f,
                .max_range = attack_range * 1.5f,
                .damage = 20.0f,
            }
        ));
        SetPlan(state, std::move(plan), 0.75f);
    }
    else
    {
        auto target = position + difference.norm() * std::min(40.0f, difference.mag());
        auto new_plan = std::make_unique<plan::Move>(core::Vector(target), 60.f);
        SetPlan(state, std::move(new_plan), .5f);
    }
}