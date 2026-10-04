#pragma once
#include "entities/Entity.hpp"
#include "plans/BasePlan.hpp"
#include "state/App.hpp"

namespace plan
{

struct TeleportConfig
{
    std::string animation_name;
    std::shared_ptr<entity::EntityHandle> target;
    std::pair<float, float> effect_time_window;
    float duration;
};

class Teleport : public BasePlan
{
public:
    float time = 0.0f;
    bool has_effected = false;

public:
    std::string animation_name;
    std::shared_ptr<entity::EntityHandle> target;
    std::pair<float, float> effect_time_window;
    float duration;

public:
    explicit Teleport(const TeleportConfig &config);
    ~Teleport() override = default;

public:
    void Start(state::App &state, entity::Entity &entity) override;
    void Update(state::App &state, entity::Entity &entity, float elapsed_time) override;
};

} // namespace plan
