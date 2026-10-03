#pragma once
#include "entities/Entity.hpp"
#include "olc/miniaudio.h"
#include "olc/olcPGEX3_Miniaudio.h"
#include "plans/BasePlan.hpp"
#include "state/App.hpp"

#include <memory>

namespace plan
{

struct AimingMissleAttackConfig
{
    std::shared_ptr<entity::EntityHandle> source;
    std::shared_ptr<entity::EntityHandle> target;
    std::shared_ptr<olc::ext::Miniaudio::Sound> sfx_cast;
    std::string animation_name;
    std::pair<float, float> cast_time_window;
    std::pair<float, float> sfx_time_window;
    float duration;
    float damage;
};

class AimingMissleAttack : public BasePlan
{
public:
    std::shared_ptr<entity::EntityHandle> source;
    std::shared_ptr<entity::EntityHandle> target;
    std::shared_ptr<olc::ext::Miniaudio::Sound> sfx_cast;
    std::string animation_name;
    float time = 0.f;
    std::pair<float, float> cast_time_window;
    std::pair<float, float> sfx_time_window;
    float duration = 0.f;
    float damage = 10.0f;
    bool has_casted = false;
    bool has_sfxed = false;

public:
    explicit AimingMissleAttack(const AimingMissleAttackConfig &config);
    ~AimingMissleAttack() override = default;

public:
    void Start(state::App &state, entity::Entity &entity) override;
    void Update(state::App &state, entity::Entity &entity, float elapsed_time) override;
};

} // namespace plan
