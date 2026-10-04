#pragma once
#include "enums/Enums.hpp"
#include "state/Card.hpp"

#include <vector>

namespace state
{

struct Player
{
public:
    static const float CHARACTER_SLOT_MAX_COOLDOWN;

public:
    float level_progress = 0.0f;
    enums::CharacterType character_type = enums::CharacterType::KNIGHT;
    enums::CharacterType character_slot_left = enums::CharacterType::KNIGHT;
    enums::CharacterType character_slot_right = enums::CharacterType::KNIGHT;
    float character_slot_left_cooldown = 0.0f;
    float character_slot_right_cooldown = 0.0f;
    float heal_cooldown = 0.0f;
    std::vector<Card> cards;
};

} // namespace state