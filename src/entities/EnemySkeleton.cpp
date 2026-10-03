#include "entities/EnemySkeleton.hpp"

#include "plans/DirectAttack.hpp"
#include "plans/Move.hpp"

using namespace entity;

EnemySkeleton::EnemySkeleton(const core::AssetManager &assets, const state::App &state, size_t id) : EnemyBase(id)
{
    health = max_health = 100.0f;
    animator = *assets.skeleton_animator;
    scale = {.8f, .8f};
    shape.size = {10.f, 10.f};
    current_animation = "walk";
    current_animation_time = 0.0f;
}

/**
 * Skeletons are medium and nothing special
 */
void EnemySkeleton::MakeNextPlan(state::App &state, float elapsed_time)
{
    const auto attack_range = 45.0f;
    auto difference = state.game_world.player_entity->ref->position - position;
    auto distance = difference.mag();

    if (distance < attack_range)
    {
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
                attack_frame = 4;
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
        plan = std::make_unique<plan::DirectAttack>(plan::DirectAttack::FromAnimationFrame(
            plan::DirectAttackFromAnimationFrameConfig{
                .source = self,
                .target = state.game_world.player_entity,
                .animation_name = attack_name,
                .hit_frame = attack_frame,
                .duration = 1.2f,
                .max_range = attack_range * 1.5f,
                .damage = 10.0f,
            }
        ));
        SetPlan(state, std::move(plan), 0.25f);
    }
    else
    {
        auto target = position + difference.norm() * std::min(75.0f, difference.mag());
        auto new_plan = std::make_unique<plan::Move>(core::Vector(target), 35.f);
        SetPlan(state, std::move(new_plan), .0f);
    }
}