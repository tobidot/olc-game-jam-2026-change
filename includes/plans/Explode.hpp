#pragma once
#include "entities/Entity.hpp"
#include "plans/BasePlan.hpp"
#include "state/App.hpp"

namespace plan
{

struct ExplodeConfig
{
    std::string animation_name;
    std::vector<enums::TargetType> targets;
    std::pair<float, float> hit_time_window;
    float range;
    float duration;
    float damage;
};

class Explode : public BasePlan
{
public:
    float time = 0.0f;
    bool has_hit = false;
    float range;
    std::string animation_name;
    std::pair<float, float> hit_time_window;
    float duration;
    float damage;
    std::vector<enums::TargetType> targets;

public:
    explicit Explode(const ExplodeConfig &config);
    ~Explode() override = default;

public:
    void Start(state::App &state, entity::Entity &entity) override;
    void Update(state::App &state, entity::Entity &entity, float elapsed_time) override;
};

} // namespace plan
