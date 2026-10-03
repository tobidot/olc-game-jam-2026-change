#pragma once
#include "core/AssetManager.hpp"
#include "entities/EnemyBase.hpp"

namespace entity
{

class EnemyWerewolf : public EnemyBase
{
public:
    EnemyWerewolf(const core::AssetManager &assets, const state::App &state, size_t id);
    ~EnemyWerewolf() override = default;

public:
    void MakeNextPlan(state::App &state, float elapsed_time) override;
};

} // namespace entity