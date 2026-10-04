#pragma once

#include "entities/Entity.hpp"
#include "plans/BasePlan.hpp"
#include "state/App.hpp"

namespace plan
{

struct DieConfig
{
    std::string animation_name;
    float duration;
};

class Die : public BasePlan
{
public:
    float time = 0.0f;

public:
    std::string animation_name;
    float duration;

public:
    explicit Die(const DieConfig &config);
    ~Die() override = default;

public:
    void Start(state::App &state, entity::Entity &entity) override;
    void Update(state::App &state, entity::Entity &entity, float elapsed_time) override;
};

} // namespace plan
