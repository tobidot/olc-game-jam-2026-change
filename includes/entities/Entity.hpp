#pragma once
#include "core/Animator.hpp"
#include "core/Geometry.hpp"
#include "olc/olcPixelGameEngine3.h"
#include "state/App.hpp"

namespace state
{
class App;
}

namespace plan
{
class BasePlan;
}

namespace entity
{

class Entity
{
public:
    bool is_removed = false;
    float health = 100.0f;
    float max_health = 100.0f;
    std::string current_animation = "idle";
    float current_animation_time = 0.0f;
    core::Vector position = core::Vector(0.f, 0.f);
    core::Vector scale = core::Vector(1.f, 1.f);
    core::Polygon shape;
    bool is_flipped = false;
    core::Animator animator;
    float plan_cooldown = 0.0f;
    std::unique_ptr<plan::BasePlan> plan;
    float damage_animation_time = 0.0f;
    float damage_animation_duration = 0.5f;

public:
    Entity();
    Entity(const Entity &cpy) = delete;
    Entity(Entity &&cpy) = delete;
    virtual ~Entity() = default;
    Entity &operator=(Entity &&other) = delete;
    Entity &operator=(const Entity &other) = delete;

public:
    virtual void Update(state::App &state, float elapsed_time);
    virtual olc::ImageRegion GetImage() const;
    virtual void Damage(float amount);
    void SetAnimation(const std::string &name);
};

} // namespace entity