#pragma once
#include "entities/Entity.hpp"
#include "enums/Enums.hpp"
#include "plans/BasePlan.hpp"

namespace entity
{

class PlayerBase : public Entity
{
public:
    enums::CharacterType type;

public:
    explicit PlayerBase(enums::CharacterType type);
    PlayerBase(const PlayerBase &cpy) = delete;
    PlayerBase(PlayerBase &&cpy) = delete;
    ~PlayerBase() override = default;
    PlayerBase &operator=(const PlayerBase &other) = delete;
    PlayerBase &operator=(PlayerBase &&other) = delete;

public:
    void Update(state::App &state, float elapsed_time) override;

public:
    virtual EntityHandle FindClosestEnemyTo(state::App &state, core::Vector position);
    virtual void DetermineNextPlan(state::App &state, float elapsed_time);
};

} // namespace entity