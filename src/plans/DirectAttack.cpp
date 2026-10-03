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
    const auto &animation = config.source->ref->animator.GetAnimation(config.animation_name);

    float hit_time = 0.0f;
    float hit_time_window = 0.0f;
    float frame_start_time = 0.f;
    float animation_speed = config.duration / animation.total_seconds;
    for (const auto &frame : animation.frames)
    {
        std::cout << "Findex " << frame.index << "\n";
        if (frame.index == config.hit_frame)
        {
            hit_time = frame_start_time * animation_speed;
            hit_time_window = frame.seconds * animation_speed;
        }
        frame_start_time += frame.seconds;
    }

    return DirectAttack(
        DirectAttackConfig{
            .target = config.target,
            .animation_name = config.animation_name,
            .hit_time = hit_time,
            .hit_time_window = hit_time_window,
            .duration = config.duration,
            .max_range = config.max_range,
            .damage = config.damage,
        }
    );
}

void DirectAttack::Start(state::App &state, entity::Entity &entity)
{
    const auto &animation = entity.animator.GetAnimation(animation_name);
    entity.SetAnimation(animation_name);
    entity.current_animation_speed = std::max(0.1f, animation.total_seconds / duration);
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

    const auto time_after_update = time + elapsed_time;
    auto difference = target->ref->position - entity.position;
    if (!has_hit)
    {
        // start time was before end of hit_time_window
        // end time is after the start of hit_time_window
        if (time < hit_time + hit_time_window && time_after_update > hit_time)
        {
            // we are on the hitting frame
            auto distance = difference.mag();
            if (distance < max_range)
            {
                target->ref->Damage(damage);
                has_hit = true;
            }
        }
    }
    entity.is_flipped = (difference.x < 0);

    time = time_after_update;
    if (time > duration)
    {
        is_finished = true;
        entity.SetNextAnimation("idle");
    }
}