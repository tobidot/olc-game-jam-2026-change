#include "plans/DirectAttack.hpp"

#include "entities/Entity.hpp"
#include "state/App.hpp"

using namespace plan;

DirectAttack::DirectAttack(const DirectAttackConfig &config)
    : target(config.target), max_range(config.max_range), hit_time(config.hit_time),
      hit_time_window(config.hit_time_window), damage(config.damage), animation_name(config.animation_name),
      duration(config.duration)
{
    name = "direct-attack";
}

DirectAttack DirectAttack::FromAnimationFrame(const DirectAttackFromAnimationFrameConfig &config)
{
    const auto &animation = (*config.target)->animator.GetAnimation(config.animation_name);

    float hit_time = 0.0f;
    float hit_time_window = 0.0f;
    float frame_start_time = 0.f;
    for (const auto &frame : animation.frames)
    {
        if (frame.index == config.hit_frame)
        {
            hit_time = frame_start_time;
            hit_time_window = frame.seconds;
        }
        frame_start_time += frame.seconds;
    }

    return DirectAttack(
        DirectAttackConfig{
            .target = config.target,
            .animation_name = config.animation_name,
            .hit_time = hit_time,
            .hit_time_window = hit_time_window,
            .max_range = config.max_range,
            .damage = config.damage,
        }
    );
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

    time += elapsed_time;
    entity.SetAnimation(animation_name);

    auto difference = (*target)->position - entity.position;
    entity.is_flipped = (difference.x < 0);
    if (time >= hit_time && time < hit_time + hit_time_window && !has_hit)
    {
        // we are on the hitting frame
        auto distance = difference.mag();
        if (distance < max_range)
        {
            (*target)->Damage(damage);
            has_hit = true;
        }
    }

    if (time > duration)
    {
        is_finished = true;
        entity.SetNextAnimation("idle");
    }
}