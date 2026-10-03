#pragma once
#include "plans/BasePlan.hpp"

#include <memory>

namespace plan
{

class ChainPlan : public BasePlan
{
public:
    size_t index = 0;
    std::vector<std::unique_ptr<BasePlan>> plans;

public:
    explicit ChainPlan(std::vector<std::unique_ptr<BasePlan>> plans);

public:
    void Start(state::App &state, entity::Entity &entity) override;
    void Update(state::App &state, entity::Entity &entity, float elapsed_time) override;
};

}; // namespace plan