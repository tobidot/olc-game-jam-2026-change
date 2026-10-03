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
    explicit PlayerBase(enums::CharacterType type, size_t id);
    PlayerBase(const PlayerBase &cpy) = delete;
    PlayerBase(PlayerBase &&cpy) = delete;
    ~PlayerBase() override = default;
    PlayerBase &operator=(const PlayerBase &other) = delete;
    PlayerBase &operator=(PlayerBase &&other) = delete;

public:
    void Update(state::App &state, float elapsed_time) override;
    void MakeNextPlan(state::App &state, float elapsed_time) override;

public:
    virtual std::shared_ptr<EntityHandle> FindClosestEnemyTo(state::App &state, core::Vector position);
};

} // namespace entity