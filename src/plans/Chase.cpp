#include "plans/Chase.hpp"

#include "entities/Entity.hpp"
#include "state/App.hpp"

using namespace plan;

Chase::Chase(const std::shared_ptr<entity::EntityHandle> &target, float velocity) : target(target), velocity(velocity)
{
    name = "chase";
}

void Chase::Start(state::App &state, entity::Entity &entity)
{
    entity.SetAnimation("walk");
}

void Chase::Update(state::App &state, entity::Entity &entity, float elapsed_time)
{
    BasePlan::Update(state, entity, elapsed_time);
    if (is_finished)
    {
        return;
    }

    auto difference_height = target->ref->GetCurrentAnchorPixelOffset("body").y - entity.z_offset;
    entity.z_offset += difference_height / 100.0f;

    auto difference = target->ref->position - entity.position;
    auto touching_distance = target->ref->shape.size.mag() + entity.shape.size.mag();

    if (difference.mag2() > touching_distance * touching_distance)
    {
        entity.position += difference.norm() * elapsed_time * velocity;
        entity.is_flipped = difference.x < 0;
    }
    else
    {
        is_finished = true;
        entity.SetAnimation("idle");
    }
}