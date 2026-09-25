#pragma once
#include "Animator.hpp"
#include "olcPixelGameEngine3.h"

#include <memory>

class AssetManager
{
public:
    // plain images
    std::shared_ptr<olc::Image> background_texture;
    std::shared_ptr<olc::Image> ships_fighter_idle_texture;
    std::shared_ptr<olc::Image> ships_fighter_boost_texture;
    // animations
    std::shared_ptr<Animator> ships_fighter;

public:
    virtual ~AssetManager() = default;

    void Load(olc::PixelGameEngine &engine);
};