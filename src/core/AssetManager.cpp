#include "AssetManager.hpp"

#include "Animator.hpp"

#include <cassert>
#include <sstream>

std::string trim(const std::string &input, char remove)
{
    auto start = input.find_first_not_of(remove);
    auto end = input.find_last_not_of(remove);
    if (start == std::string::npos || end == std::string::npos)
    {
        return {};
    }
    return input.substr(start, end + 1 - start);
}

std::string build_path(const std::string &root, const std::vector<std::string> &parts)
{
    std::stringstream buffer;
    buffer << root.substr(0, root.find_last_not_of('/') + 1);

    for (const std::string &part : parts)
    {
        buffer << '/' << trim(part, '/');
    }

    return buffer.str();
}

void AssetManager::Load(olc::PixelGameEngine &engine)
{
    // const std::string asset_path = "/media/tobidot/Libraries and Projects/assets";
    // const std::string space_background_package_path = build_path(
    //     asset_path,
    //     {std::string("licensed/commercial_use/craftpix.net/craftpix-net-410031-free-planets-in-space-pixel-game-background-pack")});
    // const std::string space_ship_package_path = build_path(
    //     asset_path,
    //     {std::string("licensed/commercial_use/craftpix.net/craftpix-net-757069-free-spaceship-pixel-art-sprite-sheets")});
    //
    const std::string asset_path = "assets";
    background_texture = std::make_shared<olc::Image>();
    engine.CreateImageFromFile(*background_texture, build_path(asset_path, {std::string("backgrounds/1.png")}));
    // background = std::make_shared<olc::ImageRegion>(*background_texture);
    //

    ships_fighter_idle_texture = std::make_shared<olc::Image>();
    engine.CreateImageFromFile(*ships_fighter_idle_texture,
                               build_path(asset_path, {std::string("ships/fighter/Idle.png")}));

    ships_fighter_boost_texture = std::make_shared<olc::Image>();
    engine.CreateImageFromFile(*ships_fighter_boost_texture,
                               build_path(asset_path, {std::string("ships/fighter/Boost.png")}));

    ships_fighter = std::make_shared<Animator>();
    ships_fighter->CreateAnimation(std::string("idle"), ships_fighter_idle_texture,
                                   {
                                       //    AnimationFrame{
                                       //        .index = 0,
                                       //        .seconds = 0.33,
                                       //        .top_left = olc::vf2d{64.0f / 192.0f, 64 / 192.0f},
                                       //        .top_right = olc::vf2d{160.0f / 192.0f, 64 / 192.0f},
                                       //        .bottom_left = olc::vf2d{64.0f / 192.0f, 128 / 192.0f},
                                       //        .bottom_right = olc::vf2d{160.0f / 192.0f, 128 / 192.0f},
                                       //    },
                                       AnimationFrame{
                                           .index = 0,
                                           .seconds = 0.33,
                                           .top_left = olc::vf2d{0.0f, 0.0f},
                                           .top_right = olc::vf2d{1.0f, 0.0f},
                                           .bottom_left = olc::vf2d{0.0f, 1.0f},
                                           .bottom_right = olc::vf2d{1.0f, 1.0f},
                                       },
                                   });
    const auto slides = 5;
    const auto slide_width = 1.0f / static_cast<float>(slides);
    // const auto offset_left = 32.0f / 192.0f / static_cast<float>(slides);
    // const auto offset_right = 160.0f / 192.0f / static_cast<float>(slides);
    // const auto offset_top = 64.0f / 192.0f;
    // const auto offset_bottom = 128.0f / 192.0f;
    const auto offset_left = 0.0f;
    const auto offset_right = 1.0f / static_cast<float>(slides);
    const auto offset_top = 0.0f;
    const auto offset_bottom = 1.0f;
    auto frames = std::vector<AnimationFrame>();
    for (size_t i = 0; i < slides; ++i)
    {
        auto left = offset_left + (slide_width * static_cast<float>(i));
        auto right = offset_right + (slide_width * static_cast<float>(i));
        auto top = offset_top;
        auto bottom = offset_bottom;
        auto frame = AnimationFrame{
            .index = i,
            .seconds = 0.33,
            .top_left = olc::vf2d{left, top},
            .top_right = olc::vf2d{right, top},
            .bottom_left = olc::vf2d{left, bottom},
            .bottom_right = olc::vf2d{right, bottom},
        };
        frames.push_back(frame);
    }
    auto animator = ships_fighter->CreateAnimation(std::string("boost"), ships_fighter_boost_texture, frames);
}
