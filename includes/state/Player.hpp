#pragma once
#include "enums/Enums.hpp"
#include "state/Card.hpp"

#include <vector>

namespace state
{

struct Player
{
    float level_progress = 0.0f;
    float switch_cooldown = 0.0f;
    enums::CharacterType character_type = enums::CharacterType::KNIGHT;
    std::vector<Card> cards;
};

} // namespace state