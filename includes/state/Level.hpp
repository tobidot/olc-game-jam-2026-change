#pragma once
#include "enums/Enums.hpp"

#include <vector>

namespace state
{

struct LevelSpawnRate
{
    std::vector<std::pair<float, std::vector<enums::EnemyType>>> spawns;
    float total_time = 0.0f;
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
