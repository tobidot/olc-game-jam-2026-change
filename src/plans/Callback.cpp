#include "plans/Channel.hpp"

#include "entities/Entity.hpp"
#include "enums/Enums.hpp"
#include "helper.hpp"
#include "plans/AimingMissleAttack.hpp"
#include "plans/ChainPlan.hpp"
#include "plans/Chase.hpp"
#include "plans/Explode.hpp"
#include "plans/Move.hpp"
#include "services/EntityService.hpp"
#include "services/RootService.hpp"
#include "services/SoundService.hpp"
#include "state/App.hpp"

using namespace plan;

Channel::Channel(const ChannelConfig &config)
    : source(config.source), target(config.target), cast_time_window(config.cast_time_window),
      sfx_time_window(config.sfx_time_window), animation_name(config.animation_name), duration(config.duration),
      sfx_cast(config.sfx_cast), on_cast(config.on_cast)
{
    name = "channel";
}

void Channel::Start(state::App &state, entity::Entity &entity)
{
    entity.SetAnimation(animation_name, duration);
}

void Channel::Update(state::App &state, entity::Entity &entity, float elapsed_time)
{
    BasePlan::Update(state, entity, elapsed_time);
    if (is_finished)
    {
        return;
    }

    auto difference = target->ref->position - entity.position;
    const auto is_in_sfx_window = IsInTimeWindow(sfx_time_window, time, elapsed_time);
    const auto is_in_cast_window = IsInTimeWindow(cast_time_window, time, elapsed_time);

    if (!has_sfxed && is_in_sfx_window)
    {
        if (sfx_cast)
        {
            service::root()->sounds->Play(*sfx_cast);
            has_sfxed = true;
        }
    }
    if (!has_casted && is_in_cast_window)
    {
        on_cast();
        has_casted = true;
    }
    entity.is_flipped = (difference.x < 0);

    time += elapsed_time;
    if (time > duration)
    {
        is_finished = true;
    }
}