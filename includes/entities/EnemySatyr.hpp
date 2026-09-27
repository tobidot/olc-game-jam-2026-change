#pragma once
#include "core/AssetManager.hpp"
#include "entities/EnemyBase.hpp"

namespace entity
{

class EnemySatyr : public EnemyBase
{
public:
    EnemySatyr(const core::AssetManager &assets, const state::App &state);
    ~EnemySatyr() override = default;
};

} // namespace entity