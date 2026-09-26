#pragma once
#include "core/Animator.hpp"
#include "core/Geometry.hpp"
#include "olc/olcPixelGameEngine3.h"
#include "state/App.hpp"

namespace state
{
class App;
}

namespace entity
{

class Entity
{
public:
    float health = 100.0f;
    float max_health = 100.0f;
    std::string current_animation = "idle";
    float current_animation_time = 0.0f;
    core::Vector position = core::Vector(0.f, 0.f);
    core::Vector scale = core::Vector(1.f, 1.f);
    core::Polygon shape;
    core::Animator animator;

public:
    virtual ~Entity() = default;

public:
    virtual void Update(state::App &state, float elapsed_time);
    virtual olc::ImageRegion GetImage() const;
};

} // namespace entity