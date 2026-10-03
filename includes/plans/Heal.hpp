#pragma once
#include "entities/Entity.hpp"
#include "plans/BasePlan.hpp"
#include "state/App.hpp"

namespace plan
{

struct HealConfig
{
    std::shared_ptr<entity::EntityHandle> target;
    std::string animation_name;
    std::pair<float, float> heal_time_window;
    float duration;
    float amount;
};

class Heal : public BasePlan
{
public:
    float time = 0.0f;
    bool has_healed = false;

public:
    std::shared_ptr<entity::EntityHandle> target;
    std::pair<float, float> heal_time_window;
    std::string animation_name;
    float duration;
    float amount;

public:
    explicit Heal(const HealConfig &config);
    ~Heal() override = default;

public:
    void Start(state::App &state, entity::Entity &entity) override;
    void Update(state::App &state, entity::Entity &entity, float elapsed_time) override;
};

} // namespace plan
