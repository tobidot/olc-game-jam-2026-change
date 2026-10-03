#pragma once
#include "entities/Entity.hpp"
#include "plans/BasePlan.hpp"

namespace entity
{

class EnemyBase : public Entity
{
public:
    EnemyBase(size_t id);
    ~EnemyBase() override = default;

public:
    void MakeNextPlan(state::App &state, float elapsed_time) override;
};

} // namespace entity