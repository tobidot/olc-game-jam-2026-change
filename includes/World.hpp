#pragma once
#include "AssetManager.hpp"
#include "GameInput.hpp"
#include "Geometry.hpp"
#include "Ship.hpp"
#include "olcPixelGameEngine3.h"

#include <memory>
#include <vector>

class Main;

class World
{

public:
    geometry::RectF boundaries;
    std::shared_ptr<Ship> player_ship;
    std::vector<std::shared_ptr<Ship>> ships;

public:
    World();
    virtual ~World();

public:
    void Seed(int generator_seed, const AssetManager &asset_manager);
    void Update(const GameInput &input, float f_elapsed_time);
    void Draw(olc::Draw &draw);
};