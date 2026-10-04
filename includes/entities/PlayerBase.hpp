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
    [[nodiscard]]
    virtual std::shared_ptr<EntityHandle>
    FindClosestEnemyTo(const state::App &state, const core::Vector &position) const;
    [[nodiscard]]
    std::vector<std::pair<float, std::shared_ptr<EntityHandle>>> GetRankedEnemies(
        const state::App &state,
        const std::function<bool(const Entity &)> &filter_by,
        const std::function<float(const Entity &)> &order_by
    ) const;
    [[nodiscard]]
    std::shared_ptr<entity::EntityHandle> GetCurrentTarget(
        const state::App &state,
        const std::function<bool(const Entity &)> &filter_by,
        const std::function<float(const Entity &)> &order_by
    ) const;

public:
    static std::function<bool(const Entity &)> FilterByDistance(const core::Vector &center, float max_distance);
    static std::function<float(const Entity &)> OrderByDistanceAsc(const core::Vector &center);
};

}; // namespace entity