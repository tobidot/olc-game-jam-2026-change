#pragma once
#include "core/Geometry.hpp"
#include "entities/EntityHandle.hpp"
#include "state/Level.hpp"
#include "state/Player.hpp"

#include <memory>
#include <vector>

namespace entity
{
class PlayerBase;
} // namespace entity

namespace state
{

struct GameWorld
{
    size_t next_entity_id = 1;
    float render_offset_top = 0.0f;
    core::RectF boundaries = {.top = 0.f, .left = 0.f, .bottom = 0.f, .right = 0.f};
    Level level;
    Player player;
    std::shared_ptr<entity::EntityHandle> player_entity;
    std::vector<std::shared_ptr<entity::EntityHandle>> entities;
    // entities currently about to spawn next frame
    std::vector<std::shared_ptr<entity::EntityHandle>> new_entities;

public:
    const std::shared_ptr<entity::EntityHandle> FindEntityHandle(size_t id);
    size_t GetNextEntityID() noexcept;
};

} // namespace state