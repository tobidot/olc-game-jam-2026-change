#pragma once
#include "core/AssetManager.hpp"
#include "entities/EnemyBase.hpp"

namespace entity
{

class EnemyMinotaur : public EnemyBase
{
public:
    EnemyMinotaur(const core::AssetManager &assets, const state::App &state, size_t id);
    ~EnemyMinotaur() override = default;

public:
    void MakeNextPlan(state::App &state, float elapsed_time) override;
};

} // namespace entity