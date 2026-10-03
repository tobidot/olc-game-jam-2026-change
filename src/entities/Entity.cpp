#include "entities/Entity.hpp"

#include "core/Geometry.hpp"
#include "exceptions/Exceptions.hpp"
#include "olc/olcPixelGameEngine3.h"
#include "plans/BasePlan.hpp"

using namespace entity;

Entity::Entity(size_t id, enums::TargetType type) : plan(nullptr), id(id), target_type(type)
{
    damage_animation_time = damage_animation_duration;
    shape = core::Polygon{.points = {}, .size = {10.f, 10.f}};
}

void Entity::Update(state::App &state, float elapsed_time)
{
    if (is_removed)
    {
        return;
    }

    // play animationss
    auto animation_total_seconds = animator.GetAnimation(current_animation).total_seconds;
    current_animation_time += elapsed_time * current_animation_speed;
    if (current_animation_time >= animation_total_seconds)
    {
        OnAnimationEnd(state);
        current_animation_time = fmodf(current_animation_time, animation_total_seconds);
    }
    if (damage_animation_time < damage_animation_duration)
    {
        damage_animation_time += elapsed_time;
    }

    if (is_dying)
    {
        // if unit is dying do not process any plan
        return;
    }

    // plan and act
    if (plan)
    {
        if (plan->is_finished)
        {
            plan.reset();
        }
        else
        {
            plan->Update(state, *this, elapsed_time);
        }
    }
    else
    {
        if (plan_cooldown <= 0.f)
        {
            MakeNextPlan(state, elapsed_time);
        }
        {
            plan_cooldown -= elapsed_time;
        }
    }

    // status updates
    if (health <= 0.0f && !is_dying)
    {
        is_dying = true;
        SetAnimation("die");
    }
}

void Entity::OnAnimationEnd(state::App &state)
{
    current_animation_speed = 1.0f;
    if (is_dying && current_animation == "die")
    {
        // at the end of the death animation
        // mark the entity as removed
        is_removed = true;
    }
    if (is_dying)
    {
        // if the entity is dying,
        // we don't want to switch to any other animation, just go for "die"
        SetAnimation("die");
    }
    if (!next_animation.empty())
    {
        // if we have a next animation set, switch to it
        SetAnimation(next_animation);
    }
}

olc::ImageRegion Entity::GetImage() const
{
    return animator.GetImage(current_animation, current_animation_time);
}

void Entity::SetAnimation(const std::string &name, float target_duration)
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
        // const auto &animation =
        current_animation_speed = animator.GetAnimationSpeedForDuration(name, target_duration);
    }
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

void Entity::MakeNextPlan(state::App &state, float elapsed_time)
{
}

void Entity::SetPlan(state::App &state, std::unique_ptr<plan::BasePlan> &&new_plan, float cooldown)
{
    plan = std::move(new_plan);
    plan->Start(state, *this);
    plan_cooldown = cooldown;
}

core::Vector Entity::GetCurrentAnchorPixelOffset(const std::string &anchor_name) const
{
    auto image = animator.GetImage(current_animation, current_animation_time);
    auto pivot = animator.GetImagePivot(current_animation, current_animation_time);
    auto anchor = animator.HasImageAnchor(current_animation, current_animation_time, anchor_name)
                      ? animator.GetImageAnchor(current_animation, current_animation_time, anchor_name)
                      : pivot;
    auto offset = anchor - pivot;
    auto offset_pixels = core::Vector(offset * image.regionsize * scale * core::Vector{-1.0f, -1.0f});

    return offset_pixels;
}