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
    std::pair<float, float> hit_time_window;
    float duration;
    float max_range;
    float damage;
};

class DirectAttack : public BasePlan
{
public:
    float time = 0.f;
    bool has_hit = false;

public:
    std::shared_ptr<entity::EntityHandle> target;
    std::string animation_name;
    std::pair<float, float> hit_time_window;
    float duration;
    float max_range;
    float damage;

public:
    explicit DirectAttack(const DirectAttackConfig &config);
    ~DirectAttack() override = default;

public:
    void Start(state::App &state, entity::Entity &entity) override;
    void Update(state::App &state, entity::Entity &entity, float elapsed_time) override;
};

} // namespace plan
