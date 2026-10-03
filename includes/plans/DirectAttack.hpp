#pragma once
#include "entities/Entity.hpp"
#include "plans/BasePlan.hpp"
#include "state/App.hpp"

#include <memory>

namespace plan
{

struct DirectAttackConfig
{
    std::shared_ptr<entity::EntityHandle> target;
    std::string animation_name;
    float hit_time;
    float hit_time_window;
    float duration;
    float max_range;
    float damage;
};

struct DirectAttackFromAnimationFrameConfig
{
    std::shared_ptr<entity::EntityHandle> source;
    std::shared_ptr<entity::EntityHandle> target;
    std::string animation_name;
    size_t hit_frame;
    float duration;
    float max_range;
    float damage;
};

class DirectAttack : public BasePlan
{
public:
    std::shared_ptr<entity::EntityHandle> target;
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
    void Start(state::App &state, entity::Entity &entity) override;
    void Update(state::App &state, entity::Entity &entity, float elapsed_time) override;
};

} // namespace plan
