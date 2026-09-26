#pragma once
#include "state/Player.hpp"

#include <vector>

namespace entity
{
class Entity;
}

namespace state
{

struct GameWorld
{
    Player player;
    std::shared_ptr<entity::Entity> player_entity;
    std::vector<std::shared_ptr<entity::Entity>> entities;
};

} // namespace state