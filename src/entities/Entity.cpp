#include "entities/Entity.hpp"

#include "olc/olcPixelGameEngine3.h"

using namespace entity;

void Entity::Update(state::App &state, float elapsed_time)
{
    auto animation_total_seconds = animator.GetAnimation(current_animation).total_seconds;
    current_animation_time = fmodf((current_animation_time + elapsed_time), animation_total_seconds);
}

olc::ImageRegion Entity::GetImage() const
{
    return animator.GetImage(current_animation, current_animation_time);
}