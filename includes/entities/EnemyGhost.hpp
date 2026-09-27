#pragma once
#include "core/AssetManager.hpp"
#include "entities/EnemyBase.hpp"

namespace entity
{

class EnemyGhost : public EnemyBase
{
public:
    EnemyGhost(const core::AssetManager &assets, const state::App &state);
    ~EnemyGhost() override = default;
};

} // namespace entity