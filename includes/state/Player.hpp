#pragma once
#include "state/Card.hpp"

#include <vector>

namespace state
{

struct Player
{
    float level_progress = 0.0f;
    float switch_cooldown = 0.0f;
    std::vector<Card> cards;
};

} // namespace state