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

    LoadKnight(engine);
    LoadGhost(engine);
    LoadMinotaur(engine);
    LoadSkeleton(engine);
    LoadSatyr(engine);
    LoadWerewolf(engine);
}

AnimationDefinition AssetManager::CreateSimpleAnimation(
    const std::shared_ptr<olc::Image> &texture,
    const std::string &name,
    const olc::vi2d &slices,
    const std::unordered_map<std::string, core::Vector> &anchors,
    const std::vector<float> &delays_per_frame,
    int frame_count
) const
{
    if (frame_count == 0)
    {
        frame_count = slices.x * slices.y;
    }
    frame_count = std::min(frame_count, slices.x * slices.y);
    auto frames = std::vector<AnimationFrameDefinition>();

    for (int32_t i = 0; i < frame_count; ++i)
    {
        const auto frame = AnimationFrameDefinition{
            .slice_index = olc::vi2d{i % slices.x, std::div(i, slices.x).quot},
            .seconds = delays_per_frame.at(i),
            .anchors = anchors,
        };
        frames.push_back(frame);
    }

    return AnimationDefinition{
        .name = name,
        .image = texture,
        .frame_slices = slices,
        .frames = frames,
    };
}

void AssetManager::LoadKnight(olc::PixelGameEngine &engine)
{
    hero_knight_idle_texture = CreateImage(engine, "knight/Knight_1/Idle.png");
    hero_knight_walk_texture = CreateImage(engine, "knight/Knight_1/Walk.png");
    // animations
    auto hero_knight_idle_animation_defintion = CreateSimpleAnimation(
        hero_knight_idle_texture,
        "idle",
        {4, 1},
        {{"pivot", {0.25f, 1.0f}}},
        {2.f, 0.16f, .16f, .66f}
    );

    auto knight_walk_anchors = std::unordered_map<std::string, core::Vector>{{"pivot", {0.25f, 1.0f}}};
    auto hero_knight_walk_animation_defintion = CreateSimpleAnimation(
        hero_knight_walk_texture,
        "walk",
        {8, 1},
        {{"pivot", {0.25f, 1.0f}}},
        {.33f, .33f, .33f, .33f, .33f, .33f, .33f, .33f}
    );
    std::cout << "Defnition Name: " << hero_knight_walk_animation_defintion.name << "\n";
    // animators
    hero_knight_animator = CreateAnimator({
        hero_knight_idle_animation_defintion,
        hero_knight_walk_animation_defintion,
    });

    for (const auto &name : hero_knight_animator->GetAnimationNames())
    {

        std::cout << "A Name: " << name << "\n";
    }
}

void AssetManager::LoadMinotaur(olc::PixelGameEngine &engine)
{
    enemy_minotaur_idle_texture = CreateImage(engine, "minotaur/Minotaur_1/Idle.png");
    enemy_minotaur_walk_texture = CreateImage(engine, "minotaur/Minotaur_1/Walk.png");
    enemy_minotaur_attack1_texture = CreateImage(engine, "minotaur/Minotaur_1/Attack.png");
    // animations
    auto idle_animation_defintion = CreateSimpleAnimation(
        enemy_minotaur_idle_texture,
        "idle",
        {10, 1},
        {{"pivot", {0.5f, 1.0f}}},
        {.33f, .33f, .33f, .33f, .33f, .33f, .33f, .33f, .33f, .33f, .33f, .33f, .33f, .33f, .33f, .33f}
    );

    auto walk_animation_defintion = CreateSimpleAnimation(
        enemy_minotaur_walk_texture,
        "walk",
        {12, 1},
        {{"pivot", {0.5f, 1.0f}}},
        {.33f, .33f, .33f, .33f, .33f, .33f, .33f, .33f, .33f, .33f, .33f, .33f, .33f, .33f, .33f, .33f}
    );

    auto attack1_animation_defintion = CreateSimpleAnimation(
        enemy_minotaur_attack1_texture,
        "attack1",
        {5, 1},
        {{"pivot", {0.25f, 1.0f}}},
        {.33f, .33f, .33f, .33f, .33f, .33f, .33f, .33f, .33f, .33f, .33f, .33f, .33f, .33f, .33f, .33f}
    );
    // animators
    enemy_minotaur_animator = CreateAnimator({
        idle_animation_defintion,
        walk_animation_defintion,
        attack1_animation_defintion,
    });
}

void AssetManager::LoadSatyr(olc::PixelGameEngine &engine)
{
    enemy_satyr_idle_texture = CreateImage(engine, "satyr/Satyr_2/Idle.png");
    enemy_satyr_walk_texture = CreateImage(engine, "satyr/Satyr_2/Walk.png");
    enemy_satyr_attack1_texture = CreateImage(engine, "satyr/Satyr_2/Attack.png");
    // animations
    auto idle_animation_defintion = CreateSimpleAnimation(
        enemy_satyr_idle_texture,
        "idle",
        {7, 1},
        {{"pivot", {0.5f, 1.0f}}},
        {.33f, .33f, .33f, .33f, .33f, .33f, .33f, .33f, .33f, .33f, .33f, .33f, .33f, .33f, .33f, .33f}
    );

    auto walk_animation_defintion = CreateSimpleAnimation(
        enemy_satyr_walk_texture,
        "walk",
        {12, 1},
        {{"pivot", {0.5f, 1.0f}}},
        {.33f, .33f, .33f, .33f, .33f, .33f, .33f, .33f, .33f, .33f, .33f, .33f, .33f, .33f, .33f, .33f}
    );

    auto attack1_animation_defintion = CreateSimpleAnimation(
        enemy_satyr_attack1_texture,
        "attack1",
        {8, 1},
        {{"pivot", {0.5f, 1.0f}}},
        {.33f, .33f, .33f, .33f, .33f, .33f, .33f, .33f, .33f, .33f, .33f, .33f, .33f, .33f, .33f, .33f}
    );
    // animators
    enemy_satyr_animator = CreateAnimator({
        idle_animation_defintion,
        walk_animation_defintion,
        attack1_animation_defintion,
    });
}

void AssetManager::LoadSkeleton(olc::PixelGameEngine &engine)
{
    enemy_skeleton_idle_texture = CreateImage(engine, "skeleton/Skeleton_Warrior/Idle.png");
    enemy_skeleton_walk_texture = CreateImage(engine, "skeleton/Skeleton_Warrior/Walk.png");
    enemy_skeleton_attack1_texture = CreateImage(engine, "skeleton/Skeleton_Warrior/Attack_1.png");
    // animations
    auto idle_animation_defintion = CreateSimpleAnimation(
        enemy_skeleton_idle_texture,
        "idle",
        {7, 1},
        {{"pivot", {0.5f, 1.0f}}},
        {.33f, .33f, .33f, .33f, .33f, .33f, .33f, .33f, .33f, .33f, .33f, .33f, .33f, .33f, .33f, .33f}
    );

    auto walk_animation_defintion = CreateSimpleAnimation(
        enemy_skeleton_walk_texture,
        "walk",
        {7, 1},
        {{"pivot", {0.5f, 1.0f}}},
        {.33f, .33f, .33f, .33f, .33f, .33f, .33f, .33f, .33f, .33f, .33f, .33f, .33f, .33f, .33f, .33f}
    );

    auto attack1_animation_defintion = CreateSimpleAnimation(
        enemy_skeleton_attack1_texture,
        "attack1",
        {5, 1},
        {{"pivot", {0.5f, 1.0f}}},
        {.33f, .33f, .33f, .33f, .33f, .33f, .33f, .33f, .33f, .33f, .33f, .33f, .33f, .33f, .33f, .33f}
    );
    // animators
    enemy_skeleton_animator = CreateAnimator({
        idle_animation_defintion,
        walk_animation_defintion,
        attack1_animation_defintion,
    });
}

void AssetManager::LoadWerewolf(olc::PixelGameEngine &engine)
{
    enemy_werewolf_idle_texture = CreateImage(engine, "werewolf/Black_Werewolf/Idle.png");
    enemy_werewolf_walk_texture = CreateImage(engine, "werewolf/Black_Werewolf/walk.png");
    enemy_werewolf_attack1_texture = CreateImage(engine, "werewolf/Black_Werewolf/Attack_1.png");
    // animations
    auto idle_animation_defintion = CreateSimpleAnimation(
        enemy_werewolf_idle_texture,
        "idle",
        {8, 1},
        {{"pivot", {0.5f, 1.0f}}},
        {.33f, .33f, .33f, .33f, .33f, .33f, .33f, .33f, .33f, .33f, .33f, .33f, .33f, .33f, .33f, .33f}
    );

    auto walk_animation_defintion = CreateSimpleAnimation(
        enemy_werewolf_walk_texture,
        "walk",
        {11, 1},
        {{"pivot", {0.5f, 1.0f}}},
        {.33f, .33f, .33f, .33f, .33f, .33f, .33f, .33f, .33f, .33f, .33f, .33f, .33f, .33f, .33f, .33f}
    );

    auto attack1_animation_defintion = CreateSimpleAnimation(
        enemy_werewolf_attack1_texture,
        "attack1",
        {6, 1},
        {{"pivot", {0.5f, 1.0f}}},
        {.33f, .33f, .33f, .33f, .33f, .33f, .33f, .33f, .33f, .33f, .33f, .33f, .33f, .33f, .33f, .33f}
    );
    // animators
    enemy_werewolf_animator = CreateAnimator({
        idle_animation_defintion,
        walk_animation_defintion,
        attack1_animation_defintion,
    });
}

void AssetManager::LoadGhost(olc::PixelGameEngine &engine)
{
    enemy_ghost_idle_texture = CreateImage(engine, "ghost/Yurei/Idle.png");
    enemy_ghost_walk_texture = CreateImage(engine, "ghost/Yurei/Walk.png");
    enemy_ghost_attack1_texture = CreateImage(engine, "ghost/Yurei/Attack_1.png");
    // animations
    auto idle_animation_defintion = CreateSimpleAnimation(
        enemy_ghost_idle_texture,
        "idle",
        {5, 1},
        {{"pivot", {0.5f, 1.0f}}},
        {0.08f, 0.16f, 0.16f, .33f, 0.5f}
    );

    auto walk_animation_defintion = CreateSimpleAnimation(
        enemy_ghost_walk_texture,
        "walk",
        {5, 1},
        {{"pivot", {0.5f, 1.0f}}},
        {.33f, .33f, .33f, .33f, .33f}
    );

    auto attack1_animation_defintion = CreateSimpleAnimation(
        enemy_ghost_attack1_texture,
        "attack1",
        {4, 1},
        {{"pivot", {0.5f, 1.0f}}},
        {.05f, .05f, .2f, .2f}
    );
    // animators
    enemy_ghost_animator = CreateAnimator({
        idle_animation_defintion,
        walk_animation_defintion,
        attack1_animation_defintion,
    });
}