#pragma once
#include "state/Account.hpp"
#include "state/GameWorld.hpp"
#include "state/Settings.hpp"

namespace entity
{
class Entity;
}

namespace state
{

struct App
{
    Settings settings;
    Account account;
    GameWorld game_world;
};

} // namespace state