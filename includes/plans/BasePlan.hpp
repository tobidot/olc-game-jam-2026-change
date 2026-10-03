#pragma once
#include "entities/Entity.hpp"
#include "state/App.hpp"

namespace plan
{

class BasePlan
{
public:
    std::string name = "unnamed";
    bool is_finished = false;

public:
    virtual ~BasePlan() = default;

public:
    virtual void Start(state::App &state, entity::Entity &entity);
    virtual void Update(state::App &state, entity::Entity &entity, float elapsed_time);

public:
    [[nodiscard]]
    bool IsInTimeWindow(std::pair<float, float> window, float now, float elapsed_time) const;
};

} // namespace plan