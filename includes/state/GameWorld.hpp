#pragma once
#include "state/Level.hpp"
#include "state/Player.hpp"

#include <memory>
#include <vector>

namespace entity
{
class Entity;
class PlayerBase;
using EntityRef = std::shared_ptr<Entity>;
using EntityHandle = std::shared_ptr<EntityRef>;
} // namespace entity

namespace state
{

struct GameWorld
{
    float render_offset_top = 0.0f;
    core::RectF boundaries = {.top = 0.f, .left = 0.f, .bottom = 0.f, .right = 0.f};
    Level level;
    Player player;
    entity::EntityHandle player_entity;
    std::vector<entity::EntityHandle> entities;
};

} // namespace state