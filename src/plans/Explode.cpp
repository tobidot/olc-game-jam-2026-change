#include "plans/Explode.hpp"

#include "entities/Entity.hpp"
#include "services/EntityService.hpp"
#include "services/RootService.hpp"
#include "state/App.hpp"

using namespace plan;

Explode::Explode(const ExplodeConfig &config)
    : animation_name(config.animation_name), duration(config.duration), hit_time_window(config.hit_time_window),
      targets(config.targets), damage(config.damage), range(config.range)
{
    name = "explode";
}

void Explode::Start(state::App &state, entity::Entity &entity)
{
    entity.SetAnimation(animation_name, duration);
}

void Explode::Update(state::App &state, entity::Entity &entity, float elapsed_time)
{
    BasePlan::Update(state, entity, elapsed_time);
    if (is_finished)
    {
        return;
    }

    const bool is_hit_window = IsInTimeWindow(hit_time_window, time, elapsed_time);

    if (!has_hit && is_hit_window)
    {
        entity.is_dying = true;
        entity.SetAnimation(animation_name);

        const auto &target_entities = service::root()->entities->Pick(entity.position, range, targets);
        for (const auto &target : target_entities)
        {
            target->ref->Damage(damage);
        }
        has_hit = true;
    }

    time += elapsed_time;
    if (time > duration)
    {
        is_finished = true;
        entity.is_removed = true;
        entity.SetNextAnimation("idle");
    }
}