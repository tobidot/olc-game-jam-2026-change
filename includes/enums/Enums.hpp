#pragma once

namespace enums
{
using uint8_t = unsigned char;

enum class CharacterType : uint8_t
{
    KNIGHT,
    SAMURAI,
    SHINOBI,
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
    VAMPIRE,
    MAX,
};

enum class EffectType : uint8_t
{
    FIREBALL,
    BLOOD,
    MAX,
};

enum class TargetType : uint8_t
{
    PLAYER,
    ENEMY,
    EFFECT,
    MAX,
};

enum class Level : uint8_t
{
    TUTORIAL,
    MAX,
};

} // namespace enums