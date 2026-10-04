#include "plans/Heal.hpp"

#include "entities/Entity.hpp"
#include "plans/Move.hpp"
#include "state/App.hpp"

using namespace plan;

Heal::Heal(const HealConfig &config)
    : target(config.target), amount(config.amount), animation_name(config.animation_name),
      heal_time_window(config.heal_time_window), duration(config.duration)
{
    name = "heal";
}

void Heal::Start(state::App &state, entity::Entity &entity)
{
    entity.SetAnimation(animation_name, duration);
}

void Heal::Update(state::App &state, entity::Entity &entity, float elapsed_time)
{
    BasePlan::Update(state, entity, elapsed_time);
    if (is_finished)
    {
        return;
    }

    const auto is_heal_time_window = IsInTimeWindow(heal_time_window, time, elapsed_time);
    if (!has_healed && is_heal_time_window)
    {
        target->ref->Heal(amount);
        has_healed = true;
    }

    time += elapsed_time;
    if (time > duration)
    {
        is_finished = true;
    }
}