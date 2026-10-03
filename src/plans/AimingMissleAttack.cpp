#include "plans/AimingMissleAttack.hpp"

#include "entities/Entity.hpp"
#include "enums/Enums.hpp"
#include "helper.hpp"
#include "plans/ChainPlan.hpp"
#include "plans/Chase.hpp"
#include "plans/Explode.hpp"
#include "plans/Move.hpp"
#include "services/EntityService.hpp"
#include "services/RootService.hpp"
#include "services/SoundService.hpp"
#include "state/App.hpp"

using namespace plan;

AimingMissleAttack::AimingMissleAttack(const AimingMissleAttackConfig &config)
    : target(config.target), cast_time_window(config.cast_time_window), sfx_time_window(config.sfx_time_window),
      damage(config.damage), animation_name(config.animation_name), duration(config.duration), sfx_cast(config.sfx_cast)
{
    name = "aiming-missle-attack";
}

void AimingMissleAttack::Start(state::App &state, entity::Entity &entity)
{
    entity.SetAnimation(animation_name, duration);
}

void AimingMissleAttack::Update(state::App &state, entity::Entity &entity, float elapsed_time)
{
    if (time <= 0)
    {
        // on first update reset animation time
        entity.current_animation_time = 0;
    }

    BasePlan::Update(state, entity, elapsed_time);
    if (is_finished)
    {
        return;
    }

    auto difference = target->ref->position - entity.position;
    const auto is_in_sfx_window = IsInTimeWindow(sfx_time_window, time, elapsed_time);
    const auto is_in_cast_window = IsInTimeWindow(cast_time_window, time, elapsed_time);

    if (!has_sfxed && is_in_sfx_window)
    {
        if (sfx_cast)
        {
            service::root()->sounds->Play(*sfx_cast);
            has_sfxed = true;
        }
    }
    if (!has_casted && is_in_cast_window)
    {
        // start time was before end of hit_time_window
        // end time is after the start of hit_time_window
        auto duration = 0.66f;
        const auto *explode_animation_name = "die";
        auto spawn_pixel_offset = entity.GetCurrentAnchorPixelOffset("weapon");

        auto spawn_position = core::Vector(entity.position + core::Vector{spawn_pixel_offset.x, 0.f});
        auto effect = service::root()->entities->SpawnEffect(enums::EffectType::FIREBALL, spawn_position);
        effect->ref->z_offset = spawn_pixel_offset.y;
        auto chase_plan = std::make_unique<plan::Chase>(target, 150.0f);
        auto hit_time_window = effect->ref->animator.GetFrameWindowTime(explode_animation_name, 5, 6) /
                               effect->ref->animator.GetAnimationSpeedForDuration(explode_animation_name, duration);
        auto explode_plan = std::make_unique<plan::Explode>(plan::ExplodeConfig{
            .animation_name = explode_animation_name,
            .targets = {enums::TargetType::PLAYER},
            .hit_time_window = hit_time_window,
            .range = 50.0f,
            .duration = duration,
            .damage = 12.f,
        });
        std::vector<std::unique_ptr<BasePlan>> plans = std::vector<std::unique_ptr<BasePlan>>();
        plans.reserve(2);
        plans.push_back(std::move(chase_plan));
        plans.push_back(std::move(explode_plan));
        auto chain_plan = std::make_unique<plan::ChainPlan>(std::move(plans));
        effect->ref->SetPlan(state, std::move(chain_plan));
        has_casted = true;
    }
    entity.is_flipped = (difference.x < 0);

    time += elapsed_time;
    if (time > duration)
    {
        is_finished = true;
        entity.SetAnimation("idle");
    }
}