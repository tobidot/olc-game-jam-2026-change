#include "World.hpp"

#include "AssetManager.hpp"
#include "GameInput.hpp"

#include <assert.h>
#include <memory>

World::World()
{
}

World::~World()
{
}

void World::Seed(int generator_seed, const AssetManager &asset_manager)
{
    player_ship = std::make_shared<Ship>(asset_manager);
    player_ship->m_position = {0.f, 0.f};
    player_ship->m_velocity = {5.f, 5.f};
    player_ship->m_angle = M_PI_2;
    ships.push_back(player_ship);

    auto other_ship = std::make_shared<Ship>(asset_manager);
    other_ship->m_position = {120.f, 0.f};
    other_ship->m_velocity = {5, 5};
    other_ship->m_angle = M_PI;
    ships.push_back(other_ship);
}

void World::Update(const GameInput &input, float f_elapsed_time)
{
    if (player_ship)
    {
        if (input.boost)
        {
            player_ship->Push(input.acceleration);
            player_ship->animator->SetAnimation("boost");
        }
        else
        {
            player_ship->animator->SetAnimation("idle");
        }
    }

    for (const auto &ship : ships)
    {
        if (ship)
        {
            ship->Update(f_elapsed_time);
        }
    }
}

void World::Draw(olc::Draw &draw)
{
    for (const auto &ship : ships)
    {
        if (ship)
        {
            ship->Draw(draw);
        }
    }
}