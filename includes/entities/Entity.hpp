#pragma once
#include "core/Animator.hpp"
#include "core/Geometry.hpp"
#include "entities/EntityHandle.hpp"
#include "enums/Enums.hpp"
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
    size_t id;

public:
    bool is_dying = false;
    bool is_removed = false;
    bool is_flipped = false;

public:
    bool can_fly = false;

public:
    enums::TargetType target_type;
    float health = 50.0f;
    float max_health = 100.0f;
    std::string current_animation = "idle";
    std::string next_animation;
    float current_animation_time = 0.0f;
    float current_animation_speed = 1.0f;
    core::Vector position = core::Vector(0.f, 0.f);
    core::Vector scale = core::Vector(1.f, 1.f);
    float z_offset = 0.0f;
    core::Polygon shape;
    core::Animator animator;
    float plan_cooldown = 0.0f;
    std::unique_ptr<plan::BasePlan> plan;
    std::shared_ptr<entity::EntityHandle> last_attack_target;
    float damage_animation_time = 0.0f;
    float damage_animation_duration = 0.16f;
    float heal_animation_time = 0.0f;
    float heal_animation_duration = 0.5f;

public:
    explicit Entity(size_t id, enums::TargetType type);
    Entity(const Entity &cpy) = delete;
    Entity(Entity &&cpy) = delete;
    virtual ~Entity() = default;
    Entity &operator=(Entity &&other) = delete;
    Entity &operator=(const Entity &other) = delete;

public:
    virtual void Update(state::App &state, float elapsed_time);
    [[nodiscard]]
    virtual olc::ImageRegion GetImage() const;
    virtual void Damage(float amount);
    virtual void Heal(float amount);
    virtual void OnAnimationEnd(state::App &state);
    virtual void MakeNextPlan(state::App &state, float elapsed_time);
    void SetAnimation(const std::string &name);
    void SetAnimation(const std::string &name, float target_duration);
    void SetNextAnimation(const std::string &name);
    /**
     * Get the offset from pivot to the provided anchor in screen pixels.
     * Falls bag to Pivot if Anchor is not found.
     */
    [[nodiscard]]
    core::Vector GetCurrentAnchorPixelOffset(const std::string &anchor_name) const;
    void SetPlan(state::App &state, std::unique_ptr<plan::BasePlan> &&new_plan, float cooldown = 1.0f);
};

} // namespace entity