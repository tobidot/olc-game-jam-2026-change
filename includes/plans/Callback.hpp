#pragma once

#include "entities/Entity.hpp"
#include "olc/miniaudio.h"
#include "olc/olcPGEX3_Miniaudio.h"
#include "plans/BasePlan.hpp"
#include "state/App.hpp"

#include <memory>

namespace plan
{

class Callback : public BasePlan
{
public:
public:
    std::function<void()> callback;

public:
    explicit Callback(const std::function<void()> &callback);
    ~Callback() override = default;

public:
    void Start(state::App &state, entity::Entity &entity) override;
    void Update(state::App &state, entity::Entity &entity, float elapsed_time) override;
};

} // namespace plan
