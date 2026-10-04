#include "plans/Teleport.hpp"

#include "entities/Entity.hpp"
#include "state/App.hpp"

using namespace plan;

Teleport::Teleport(const TeleportConfig &config)
    : target(config.target), effect_time_window(config.effect_time_window), animation_name(config.animation_name),
      duration(config.duration)
{
    name = "Teleport";
}

void Teleport::Start(state::App &state, entity::Entity &entity)
{
    entity.SetAnimation(animation_name, duration);
}

void Teleport::Update(state::App &state, entity::Entity &entity, float elapsed_time)
{
    BasePlan::Update(state, entity, elapsed_time);
    if (is_finished)
    {
        return;
    }
    const bool is_in_effect_time_window = IsInTimeWindow(effect_time_window, time, elapsed_time);
    if (!has_effected && is_in_effect_time_window)
    {
        auto offset = core::Vector{10.f, 5.f};
        entity.position = core::Vector(target->ref->position + offset);

        has_effected = true;
    }

    time += elapsed_time;
    if (time > duration)
    {
        is_finished = true;
    }
}