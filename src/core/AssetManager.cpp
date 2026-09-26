#include "core/AssetManager.hpp"

#include "core/Animator.hpp"

#include <cassert>
#include <sstream>

using namespace core;

std::string_view AssetManager::Trim(const std::string_view &input, char remove) const
{
    auto start = input.find_first_not_of(remove);
    auto end = input.find_last_not_of(remove);
    if (start == std::string::npos || end == std::string::npos)
    {
        return {};
    }
    return input.substr(start, end + 1 - start);
}

std::shared_ptr<olc::Image> AssetManager::CreateImage(olc::PixelGameEngine &engine, const char *path) const
{
    auto image = std::make_shared<olc::Image>();
    auto string_path = MakeAssetPath(path);
    engine.CreateImageFromFile(*image, string_path);
    return image;
}

std::shared_ptr<Animator> AssetManager::CreateAnimator(const std::vector<AnimationDefinition> &animations) const
{
    auto animator = std::make_shared<Animator>();

    for (const auto &animation : animations)
    {
        auto frames = std::vector<AnimationFrame>();
        const auto frame_count = animation.frames.size();
        const auto slide_width = 1.0f / static_cast<float>(animation.frame_slices.x);
        const auto slide_height = 1.0f / static_cast<float>(animation.frame_slices.y);
        for (size_t i = 0; i < frame_count; ++i)
        {
            const auto &frame_definition = animation.frames.at(i);
            auto left = (slide_width * static_cast<float>(frame_definition.slice_index.x));
            auto right = (slide_width * static_cast<float>(frame_definition.slice_index.x + 1));
            auto top = (slide_height * static_cast<float>(frame_definition.slice_index.y));
            auto bottom = (slide_height * static_cast<float>(frame_definition.slice_index.y + 1));
            auto anchors = std::unordered_map<std::string, core::Vector>();
            anchors["pivot"] = {0.5f, 0.5f};
            for (const auto &anchor : frame_definition.anchors)
            {
                anchors[anchor.first] = core::Vector(anchor.second);
            }
            auto frame = AnimationFrame{
                .index = i,
                .seconds = frame_definition.seconds,
                .anchors = anchors,
                .top_left = core::Vector{left, top},
                .top_right = core::Vector{right, top},
                .bottom_left = core::Vector{left, bottom},
                .bottom_right = core::Vector{right, bottom},
            };
            frames.push_back(frame);
        }

        animator->CreateAnimation(animation.name, animation.image, frames);
    }

    return animator;
}

void AssetManager::Load(olc::PixelGameEngine &engine)
{
    // images
    background_jungle_texture = CreateImage(engine, "backgrounds/PNG/Battleground4/Pale/Battleground4.png");
    hero_knight_idle_texture = CreateImage(engine, "knight/Knight_1/Idle.png");
    hero_knight_walk_texture = CreateImage(engine, "knight/Knight_1/Walk.png");
    // animations
    auto hero_knight_idle_animation_defintion = AnimationDefinition{
        .name = "idle",
        .image = hero_knight_idle_texture,
        .frame_slices = {4, 1},
        .frames = {
            {.slice_index = {0, 0}, .seconds = 2.00f},
            {.slice_index = {1, 0}, .seconds = 0.16f},
            {.slice_index = {2, 0}, .seconds = 0.16f},
            {.slice_index = {3, 0}, .seconds = 0.66f},
        },
    };
    auto hkwkad_anchors = std::unordered_map<std::string, core::Vector>{{"pivot", {0.25f, 1.0f}}};
    auto hero_knight_walk_animation_defintion = AnimationDefinition{
        .name = "walk",
        .image = hero_knight_walk_texture,
        .frame_slices = {8, 1},
        .frames = {
            {.slice_index = {0, 0}, .seconds = 0.33f, .anchors = hkwkad_anchors},
            {.slice_index = {1, 0}, .seconds = 0.33f, .anchors = hkwkad_anchors},
            {.slice_index = {2, 0}, .seconds = 0.33f, .anchors = hkwkad_anchors},
            {.slice_index = {3, 0}, .seconds = 0.33f, .anchors = hkwkad_anchors},
            {.slice_index = {4, 0}, .seconds = 0.33f, .anchors = hkwkad_anchors},
            {.slice_index = {5, 0}, .seconds = 0.33f, .anchors = hkwkad_anchors},
            {.slice_index = {6, 0}, .seconds = 0.33f, .anchors = hkwkad_anchors},
            {.slice_index = {7, 0}, .seconds = 0.33f, .anchors = hkwkad_anchors},
        },
    };
    // animators
    hero_knight_animator = CreateAnimator({
        hero_knight_idle_animation_defintion,
        hero_knight_walk_animation_defintion,
    });
}
