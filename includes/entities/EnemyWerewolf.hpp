#pragma once
#include "core/AssetManager.hpp"
#include "entities/EnemyBase.hpp"

namespace entity
{

class EnemyWerewolf : public EnemyBase
{
public:
    EnemyWerewolf(const core::AssetManager &assets, const state::App &state);
    ~EnemyWerewolf() override = default;
};

} // namespace entity