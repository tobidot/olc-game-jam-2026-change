#include "plans/Die.hpp"

#include "entities/Entity.hpp"
#include "plans/Explode.hpp"
#include "services/EntityService.hpp"
#include "services/RootService.hpp"
#include "state/App.hpp"

using namespace plan;

Die::Die(const DieConfig &config) : animation_name(config.animation_name), duration(config.duration)
{
    name = "die";
}

void Die::Start(state::App &state, entity::Entity &entity)
{
    entity.SetAnimation(animation_name, duration);
}

void Die::Update(state::App &state, entity::Entity &entity, float elapsed_time)
{
    BasePlan::Update(state, entity, elapsed_time);
    if (is_finished)
    {
        return;
    }

    time += elapsed_time;
    if (time > duration)
    {
        is_finished = true;
        entity.is_removed = true;
    }
}