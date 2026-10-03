#pragma once

#include "core/AssetManager.hpp"
#include "entities/Entity.hpp"
#include "enums/Enums.hpp"
#include "state/App.hpp"

#include <memory>

namespace service
{

class EntityService
{
private:
    const core::AssetManager *assets;
    state::App *state;

public:
    EntityService(const core::AssetManager *assets, state::App *state);
    virtual ~EntityService() = default;
    [[nodiscard]]
    std::shared_ptr<entity::EntityHandle> SpawnEnemy(enums::EnemyType type) const;
    [[nodiscard]]
    std::shared_ptr<entity::EntityHandle> SpawnEffect(enums::EffectType type, const core::Vector &position) const;
    /**
     * Find all entity in a radius.
     * May filter by target type
     */
    [[nodiscard]]
    std::vector<std::shared_ptr<entity::EntityHandle>>
    Pick(core::Vector center, float distance, const std::vector<enums::TargetType> &target_types = {}) const;
};

}; // namespace service