#pragma once
#include "entities/Entity.hpp"
#include "plans/BasePlan.hpp"
#include "state/App.hpp"

namespace plan
{

class Move : public BasePlan
{
public:
    core::Vector target;
    float velocity;

public:
    explicit Move(const core::Vector &target, float velocity);
    ~Move() override = default;

public:
    void Update(state::App &state, entity::Entity &entity, float elapsed_time) override;
};

} // namespace plan
