#pragma once
#include "core/AssetManager.hpp"
#include "entities/EnemyBase.hpp"

namespace entity
{

class EnemyVampire : public EnemyBase
{
public:
    EnemyVampire(const core::AssetManager &assets, const state::App &state, size_t id);
    ~EnemyVampire() override = default;
};

} // namespace entity