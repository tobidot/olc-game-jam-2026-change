#include "entities/EffectPiercingLight.hpp"

#include "core/AssetManager.hpp"
#include "services/EntityService.hpp"
#include "services/RootService.hpp"
#include "state/App.hpp"

using namespace entity;

EffectPiercingLight::EffectPiercingLight(const core::AssetManager &assets, const state::App &state, size_t id)
    : Effect(assets, state, id)
{
    health = max_health = 25.0f;
    animator = *assets.wizard_missle1_animator;
    shape.size = {10.f, 10.f};
    scale = {.45f, .45f};
    z_offset = 15.0f;
    current_animation = "idle";
    current_animation_time = 0.0f;
}

EffectPiercingLight::~EffectPiercingLight()
{
    has_hit_enemies.clear();
}

void EffectPiercingLight::Update(state::App &state, float elapsed_time)
{
    Effect::Update(state, elapsed_time);

    const auto enemies = service::root()->entities->Pick(position, 35.f, {enums::TargetType::ENEMY});

    for (const auto &enemy : enemies)
    {
        if (!enemy->ref || enemy->ref->is_dying || enemy->ref->is_removed)
        {
            continue;
        }
        const bool has_hit = has_hit_enemies.contains(enemy->ref->id);
        if (!has_hit)
        {
            enemy->ref->Damage(damage);
            has_hit_enemies[enemy->ref->id] = enemy;
        }
    }
}

void EffectPiercingLight::MakeNextPlan(state::App &state, float elapsed_time)
{
    Effect::MakeNextPlan(state, elapsed_time);
}