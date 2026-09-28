#pragma once
#include "state/Level.hpp"
#include "state/Player.hpp"

#include <vector>

namespace entity
{
class Entity;
class PlayerBase;
} // namespace entity

namespace state
{

struct GameWorld
{
    float render_offset_top = 0.0f;
    core::RectF boundaries = {.top = 0.f, .left = 0.f, .bottom = 0.f, .right = 0.f};
    Level level;
    Player player;
    std::shared_ptr<std::shared_ptr<entity::Entity>> player_entity;
    std::vector<std::shared_ptr<entity::Entity>> entities;
};

} // namespace state