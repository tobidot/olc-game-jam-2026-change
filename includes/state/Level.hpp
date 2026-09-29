#pragma once
#include "enums/Enums.hpp"

#include <vector>

namespace state
{

struct LevelSpawnRate
{
    float total_time = 0.0f;
    std::vector<std::pair<float, enums::EnemyType>> spawns;
};

struct LevelEvent
{
    float at_progress = 0.f;
    std::vector<enums::EnemyType> spawns;
    LevelSpawnRate new_spawn_rate;
};

class Level
{
public:
    float spawn_rate_time = 0.f;
    LevelSpawnRate spawn_rate;
    std::vector<LevelEvent> events;
};

} // namespace state
