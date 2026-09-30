#include "entities/Entity.hpp"

#include "exceptions/Exceptions.hpp"
#include "olc/olcPixelGameEngine3.h"
#include "plans/BasePlan.hpp"

using namespace entity;

Entity::Entity() : plan(nullptr)
{
    damage_animation_time = damage_animation_duration;
}

void Entity::Update(state::App &state, float elapsed_time)
{
    auto animation_total_seconds = animator.GetAnimation(current_animation).total_seconds;
    if (current_animation_time + elapsed_time >= animation_total_seconds && !next_animation.empty())
    {
        SetAnimation(next_animation);
    }
    current_animation_time = fmodf((current_animation_time + elapsed_time), animation_total_seconds);
    if (damage_animation_time < damage_animation_duration)
    {
        damage_animation_time += elapsed_time;
    }

    if (plan && plan->is_finished)
    {
        plan.reset();
    }

    if (health <= 0.0f)
    {
        // todo DIE
        is_removed = true;
    }
}

olc::ImageRegion Entity::GetImage() const
{
    return animator.GetImage(current_animation, current_animation_time);
}

void Entity::SetAnimation(const std::string &name)
{
    const auto &allowed_animation = animator.GetAnimationNames();
    const auto exists = std::ranges::find(allowed_animation, name) != allowed_animation.end();
    if (!exists)
    {
        throw exceptions::runtime::AnimationIndexNotFoundException(name);
    }

    const auto is_new_animation = (name != current_animation);
    current_animation = name;
    next_animation = "";
    if (is_new_animation)
    {
        // reset timer
        current_animation_time = 0.0f;
    }
}

void Entity::SetNextAnimation(const std::string &name)
{
    const auto &allowed_animation = animator.GetAnimationNames();
    const auto exists = std::ranges::find(allowed_animation, name) != allowed_animation.end();
    if (!exists)
    {
        throw exceptions::runtime::AnimationIndexNotFoundException(name);
    }

    next_animation = name;
}

void Entity::Damage(float amount)
{
    health -= amount;
    damage_animation_time = 0;
}