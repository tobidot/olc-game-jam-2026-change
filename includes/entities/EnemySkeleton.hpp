#pragma once
#include "core/AssetManager.hpp"
#include "entities/EnemyBase.hpp"

namespace entity
{

class EnemySkeleton : public EnemyBase
{
public:
    EnemySkeleton(const core::AssetManager &assets, const state::App &state, size_t id);
    ~EnemySkeleton() override = default;

public:
    void MakeNextPlan(state::App &state, float elapsed_time) override;
};

} // namespace entity