#pragma once
#include <inttypes.h>

namespace enums
{

enum class CharacterType : uint8_t
{
    KNIGHT,
    SAMURAI,
    SHINOBI,
    VAMPIRE,
    WIZARD,
    MAX,
};

enum class EnemyType : uint8_t
{
    GHOST,
    MINOTAUR,
    SATYR,
    SKELETON,
    WEREWOLF,
    MAX,
};

enum class Level : uint8_t
{
    TUTORIAL,
    MAX,
};

} // namespace enums