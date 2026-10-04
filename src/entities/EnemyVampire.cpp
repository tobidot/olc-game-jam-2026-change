#include "entities/EnemyVampire.hpp"

#include "helper.hpp"
#include "plans/AimingMissleAttack.hpp"
#include "plans/BasePlan.hpp"
#include "plans/Callback.hpp"
#include "plans/ChainPlan.hpp"
#include "plans/Channel.hpp"
#include "plans/Chase.hpp"
#include "plans/Die.hpp"
#include "plans/DirectAttack.hpp"
#include "plans/Explode.hpp"
#include "plans/Heal.hpp"
#include "plans/Move.hpp"
#include "services/EntityService.hpp"
#include "services/RootService.hpp"

using namespace entity;

EnemyVampire::EnemyVampire(const core::AssetManager &assets, const state::App &state, size_t id) : EnemyBase(id)
{
    health = max_health = 70.0f;
    animator = *assets.vampire_animator;
    scale = {.8f, .8f};
    current_animation = "idle";
    current_animation_time = 0.0f;
}

/**
 * Vampire damage check healing it self on dealing damage
 */
void EnemyVampire::MakeNextPlan(state::App &state, float elapsed_time)
{
    const auto attack_range = 250.0f;
    const auto &player = state.game_world.player_entity;
    auto difference = player->ref->position - position;
    auto distance = difference.mag();

    if (distance < attack_range)
    {
        if (!is_blood_available)
        {
            // idle wait for the blood
            return;
        }
        auto duration = 1.3f;
        std::string attack_name = "attack1";
        size_t attack_frame = 5;

        auto self = state.game_world.FindEntityHandle(id);
        auto animation_speed = animator.GetAnimationSpeedForDuration(attack_name, duration);
        auto cast_time_window = animator.GetFrameWindowTime(attack_name, attack_frame, attack_frame) / animation_speed;
        auto sfx_time_window = animator.GetFrameWindowTime(attack_name, 1, attack_frame) / animation_speed;

        auto on_cast = [this] { this->OnCast(); };
        plan = std::make_unique<plan::Channel>(plan::ChannelConfig{
            .source = self,
            .target = player,
            .sfx_cast = service::root()->assets->sfx_swarp,
            .animation_name = attack_name,
            .cast_time_window = cast_time_window,
            .sfx_time_window = sfx_time_window,
            .duration = duration,
            .on_cast = on_cast,
        });
        SetPlan(state, std::move(plan), 0.33f);
        return;
    }

    // move towards player
    auto target = position + difference.norm() * std::min(30.0f, difference.mag());
    auto new_plan = std::make_unique<plan::Move>(core::Vector(target), 40.f);
    SetPlan(state, std::move(new_plan), .25f);
}

void EnemyVampire::OnCast()
{
    is_blood_available = false;

    auto *state = service::root()->state;
    const auto attack_range = 250.0f;
    const auto &player = state->game_world.player_entity;
    auto self = state->game_world.FindEntityHandle(id);

    auto duration = 0.45f;
    size_t attack_frame = 5;

    auto spawn_pixel_offset = GetCurrentAnchorPixelOffset("weapon");
    auto spawn_position = core::Vector(position + core::Vector{spawn_pixel_offset.x, 0.f});
    auto effect = service::root()->entities->SpawnEffect(enums::EffectType::BLOOD, spawn_position);
    effect->ref->z_offset = spawn_pixel_offset.y;
    auto chase_plan = std::make_unique<plan::Chase>(player, 150.0f);
    auto chase_back_plan = std::make_unique<plan::Chase>(self, 80.0f);
    //     std::shared_ptr<entity::EntityHandle> target;

    std::string damage_animation_name = "idle";
    float damage_duration = 0.1f;
    float damage_animation_speed =
        effect->ref->animator.GetAnimationSpeedForDuration(damage_animation_name, damage_duration);
    std::pair<float, float> damage_time_window =
        effect->ref->animator.GetFrameWindowTime(damage_animation_name, 0, 0) / damage_animation_speed;
    auto damage_plan = std::make_unique<plan::DirectAttack>(plan::DirectAttackConfig{
        .target = player,
        .animation_name = damage_animation_name,
        .hit_time_window = damage_time_window,
        .duration = damage_duration,
        .max_range = 100.0f,
        .damage = 10.f,
    });

    std::string heal_animation_name = "idle";
    float heal_duration = 0.1f;
    float heal_animation_speed = effect->ref->animator.GetAnimationSpeedForDuration(heal_animation_name, heal_duration);
    std::pair<float, float> heal_time_window =
        effect->ref->animator.GetFrameWindowTime(heal_animation_name, 0, 0) / heal_animation_speed;
    auto heal_plan = std::make_unique<plan::Heal>(plan::HealConfig{
        .target = self,
        .animation_name = heal_animation_name,
        .heal_time_window = heal_time_window,
        .duration = heal_duration,
        .amount = 20.f,
    });
    auto restore_blood_plan = std::make_unique<plan::Callback>([this] { this->is_blood_available = true; });
    auto die_plan = std::make_unique<plan::Die>(plan::DieConfig{
        .animation_name = "die",
        .duration = 0.33f,
    });
    std::vector<std::unique_ptr<plan::BasePlan>> plans = std::vector<std::unique_ptr<plan::BasePlan>>();
    plans.reserve(2);
    plans.push_back(std::move(chase_plan));
    plans.push_back(std::move(damage_plan));
    plans.push_back(std::move(chase_back_plan));
    plans.push_back(std::move(heal_plan));
    plans.push_back(std::move(restore_blood_plan));
    plans.push_back(std::move(die_plan));
    auto chain_plan = std::make_unique<plan::ChainPlan>(std::move(plans));
    effect->ref->SetPlan(*state, std::move(chain_plan));
}
