#pragma once
#include "core/AssetManager.hpp"
#include "entities/EnemyBase.hpp"

namespace entity
{

class EnemyMinotaur : public EnemyBase
{
public:
    EnemyMinotaur(const core::AssetManager &assets, const state::App &state);
    ~EnemyMinotaur() override = default;
};

} // namespace entity