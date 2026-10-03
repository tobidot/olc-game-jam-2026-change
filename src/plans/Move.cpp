#include "plans/Move.hpp"

#include "entities/Entity.hpp"
#include "state/App.hpp"

using namespace plan;

Move::Move(const core::Vector &target, float velocity) : target(target), velocity(velocity)
{
    name = "move";
}

void Move::Start(state::App &state, entity::Entity &entity)
{
    entity.SetAnimation("walk");
}

void Move::Update(state::App &state, entity::Entity &entity, float elapsed_time)
{
    BasePlan::Update(state, entity, elapsed_time);
    if (is_finished)
    {
        return;
    }

    auto difference = target - entity.position;
    if (difference.mag2() > 10.0f)
    {
        entity.position += difference.norm() * elapsed_time * velocity;
        entity.is_flipped = difference.x < 0;

        // check if walking against the endge
        if (!state.game_world.boundaries.Contains(entity.position) && !state.game_world.boundaries.Contains(target))
        {
            is_finished = true;
        }
        return;
    }

    is_finished = true;
}