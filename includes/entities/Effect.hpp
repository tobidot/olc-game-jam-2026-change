#pragma once

#include "core/AssetManager.hpp"
#include "entities/Entity.hpp"
#include "plans/BasePlan.hpp"
#include "state/App.hpp"

#include <memory>

namespace entity
{

class Effect : public Entity
{
public:
    explicit Effect(const core::AssetManager &assets, const state::App &state, size_t id);

public:
    void Update(state::App &state, float elapsed_time) override;
};

} // namespace entity