#pragma once
#include "entities/Entity.hpp"
#include "plans/BasePlan.hpp"

namespace entity
{

class EnemyBase : public Entity
{
public:
    ~EnemyBase() override = default;

public:
    void Update(state::App &state, float elapsed_time) override;
};

} // namespace entity