#pragma once
#include "entities/Entity.hpp"
#include "plans/BasePlan.hpp"
#include "state/App.hpp"

namespace plan
{

class Chase : public BasePlan
{
public:
    std::shared_ptr<entity::EntityHandle> target;
    float velocity;

public:
    explicit Chase(const std::shared_ptr<entity::EntityHandle> &target, float velocity);
    ~Chase() override = default;

public:
    void Start(state::App &state, entity::Entity &entity) override;
    void Update(state::App &state, entity::Entity &entity, float elapsed_time) override;
};

} // namespace plan