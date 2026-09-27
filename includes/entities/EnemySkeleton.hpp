#pragma once
#include "core/AssetManager.hpp"
#include "entities/EnemyBase.hpp"

namespace entity
{

class EnemySkeleton : public EnemyBase
{
public:
    EnemySkeleton(const core::AssetManager &assets, const state::App &state);
    ~EnemySkeleton() override = default;
};

} // namespace entity