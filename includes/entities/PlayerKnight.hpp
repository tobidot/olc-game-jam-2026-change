#pragma once

#include "core/AssetManager.hpp"
#include "entities/Entity.hpp"
#include "state/App.hpp"

namespace entity
{

class PlayerKnight : public Entity
{
public:
    PlayerKnight(const core::AssetManager &assets, const state::App &state);
    PlayerKnight(const PlayerKnight &cpy) = default;
    PlayerKnight(PlayerKnight &&cpy) = default;
    ~PlayerKnight() override = default;
    PlayerKnight &operator=(const PlayerKnight &other) = default;
    PlayerKnight &operator=(PlayerKnight &&other) = default;

public:
    void Update(state::App &state, float elapsed_time) override;
};

} // namespace entity