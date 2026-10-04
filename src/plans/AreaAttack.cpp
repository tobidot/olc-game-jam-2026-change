#include "plans/AreaAttack.hpp"

#include "entities/Entity.hpp"
#include "services/EntityService.hpp"
#include "services/RootService.hpp"
#include "services/SoundService.hpp"
#include "state/App.hpp"

using namespace plan;

AreaAttack::AreaAttack(const AreaAttackConfig &config)
    : animation_name(config.animation_name), duration(config.duration), hit_time_window(config.hit_time_window),
      source(config.source), targets(config.targets), damage(config.damage), target(config.target), area(config.area),
      sfx_hit(config.sfx_hit)
{
    name = "AreaAttack";
}

void AreaAttack::Start(state::App &state, entity::Entity &entity)
{
    entity.SetAnimation(animation_name, duration);
}

void AreaAttack::Update(state::App &state, entity::Entity &entity, float elapsed_time)
{
    BasePlan::Update(state, entity, elapsed_time);
    if (is_finished)
    {
        return;
    }

    const bool is_hit_window = IsInTimeWindow(hit_time_window, time, elapsed_time);

    if (!has_hit && is_hit_window)
    {
        const auto &target_entities = service::root()->entities->Pick(target, area, targets);
        for (const auto &target : target_entities)
        {
            target->ref->Damage(damage);
        }
        has_hit = true;

        service::root()->sounds->Play(*sfx_hit);
    }

    time += elapsed_time;
    if (time > duration)
    {
        is_finished = true;
    }
}