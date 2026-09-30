#pragma once
#include "entities/Entity.hpp"
#include "plans/BasePlan.hpp"
#include "state/App.hpp"

namespace plan
{

struct DirectAttackConfig
{
    entity::EntityHandle target;
    std::string animation_name;
    float hit_time;
    float hit_time_window;
    float duration;
    float max_range;
    float damage;
};

struct DirectAttackFromAnimationFrameConfig
{
    entity::EntityHandle target;
    std::string animation_name;
    size_t hit_frame;
    float max_range;
    float damage;
};

class DirectAttack : public BasePlan
{
public:
    entity::EntityHandle target;
    std::string animation_name;
    float time = 0.f;
    float hit_time = 0.f;
    float hit_time_window = 0.f;
    float duration = 0.f;
    float max_range = 100.0f;
    float damage = 10.0f;
    bool has_hit = false;

public:
    explicit DirectAttack(const DirectAttackConfig &config);
    ~DirectAttack() override = default;

    static DirectAttack FromAnimationFrame(const DirectAttackFromAnimationFrameConfig &config);

public:
    void Update(state::App &state, entity::Entity &entity, float elapsed_time) override;
};

} // namespace plan
