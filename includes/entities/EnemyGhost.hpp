#pragma once
#include "core/AssetManager.hpp"
#include "entities/EnemyBase.hpp"

namespace entity
{

class EnemyGhost : public EnemyBase
{
public:
    EnemyGhost(const core::AssetManager &assets, const state::App &state, size_t id);
    ~EnemyGhost() override = default;

public:
    void MakeNextPlan(state::App &state, float elapsed_time) override;
};

} // namespace entity