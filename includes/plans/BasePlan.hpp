#pragma once
#include "entities/Entity.hpp"
#include "state/App.hpp"

namespace plan
{

class BasePlan
{
public:
    bool is_finished = false;

public:
    virtual ~BasePlan() = default;

public:
    virtual void Update(state::App &state, entity::Entity &entity, float elapsed_time);
};

} // namespace plan