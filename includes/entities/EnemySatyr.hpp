#pragma once
#include "core/AssetManager.hpp"
#include "entities/EnemyBase.hpp"

namespace entity
{

class EnemySatyr : public EnemyBase
{
public:
    EnemySatyr(const core::AssetManager &assets, const state::App &state, size_t id);
    ~EnemySatyr() override = default;

public:
    void MakeNextPlan(state::App &state, float elapsed_time) override;
};

} // namespace entity