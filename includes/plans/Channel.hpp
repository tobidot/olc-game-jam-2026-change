#pragma once

#include "entities/Entity.hpp"
#include "olc/miniaudio.h"
#include "olc/olcPGEX3_Miniaudio.h"
#include "plans/BasePlan.hpp"
#include "state/App.hpp"

#include <memory>

namespace plan
{

struct ChannelConfig
{
    std::shared_ptr<entity::EntityHandle> source;
    std::shared_ptr<entity::EntityHandle> target;
    std::shared_ptr<olc::ext::Miniaudio::Sound> sfx_cast;
    std::string animation_name;
    std::pair<float, float> cast_time_window;
    std::pair<float, float> sfx_time_window;
    float duration;
    std::function<void()> on_cast;
};

class Channel : public BasePlan
{
public:
    bool has_casted = false;
    bool has_sfxed = false;
    float time = 0.f;

public:
    std::shared_ptr<entity::EntityHandle> source;
    std::shared_ptr<entity::EntityHandle> target;
    std::shared_ptr<olc::ext::Miniaudio::Sound> sfx_cast;
    std::string animation_name;
    std::pair<float, float> cast_time_window;
    std::pair<float, float> sfx_time_window;
    float duration;
    std::function<void()> on_cast;

public:
    explicit Channel(const ChannelConfig &config);
    ~Channel() override = default;

public:
    void Start(state::App &state, entity::Entity &entity) override;
    void Update(state::App &state, entity::Entity &entity, float elapsed_time) override;
};

} // namespace plan
