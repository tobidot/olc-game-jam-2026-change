#include "plans/DirectAttack.hpp"

#include "entities/Entity.hpp"
#include "olc/miniaudio.h"
#include "olc/olcPGEX3_Miniaudio.h"
#include "services/RootService.hpp"
#include "services/SoundService.hpp"
#include "state/App.hpp"

using namespace plan;

DirectAttack::DirectAttack(const DirectAttackConfig &config)
    : target(config.target), max_range(config.max_range), hit_time_window(config.hit_time_window),
      damage(config.damage), animation_name(config.animation_name), duration(config.duration), sfx_hit(config.sfx_hit)
{
    name = "direct-attack";
}

void DirectAttack::Start(state::App &state, entity::Entity &entity)
{
    entity.SetAnimation(animation_name, duration);
}

void DirectAttack::Update(state::App &state, entity::Entity &entity, float elapsed_time)
{
    if (time <= 0)
    {
        // on first update reset animation time
        entity.current_animation_time = 0;
    }

    BasePlan::Update(state, entity, elapsed_time);
    if (is_finished)
    {
        return;
    }

    auto difference = target->ref->position - entity.position;
    auto is_hit_time_window = IsInTimeWindow(hit_time_window, time, elapsed_time);
    if (!has_hit && is_hit_time_window)
    {
        // we are on the hitting frame
        auto distance = difference.mag();
        if (distance < max_range)
        {
            target->ref->Damage(damage);
            has_hit = true;

            if (sfx_hit)
            {
                service::root()->sounds->Play(*sfx_hit);
            }
        }
    }
    entity.is_flipped = (difference.x < 0);

    time += elapsed_time;
    if (time > duration)
    {
        is_finished = true;
    }
}