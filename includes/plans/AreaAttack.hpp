#pragma once
#include "entities/Entity.hpp"
#include "plans/BasePlan.hpp"
#include "state/App.hpp"

#include <memory>

namespace plan
{

struct AreaAttackConfig
{
    std::shared_ptr<entity::EntityHandle> source;
    std::string animation_name;
    std::pair<float, float> hit_time_window;
    float duration;
    core::Vector target;
    std::vector<enums::TargetType> targets;
    float area;
    float damage;
};

class AreaAttack : public BasePlan
{
public:
    float time = 0.f;
    bool has_hit = false;

public:
    std::shared_ptr<entity::EntityHandle> source;
    std::string animation_name;
    std::pair<float, float> hit_time_window;
    float duration;
    core::Vector target;
    std::vector<enums::TargetType> targets;
    float area;
    float damage;

public:
    explicit AreaAttack(const AreaAttackConfig &config);
    ~AreaAttack() override = default;

public:
    void Start(state::App &state, entity::Entity &entity) override;
    void Update(state::App &state, entity::Entity &entity, float elapsed_time) override;
};

} // namespace plan
