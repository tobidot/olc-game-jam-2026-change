#include "core/AssetManager.hpp"

#include "core/Animator.hpp"
#include "olc/olcPGEX3_Miniaudio.h"

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

void AssetManager::Load(olc::PixelGameEngine &engine, olc::ext::Miniaudio::AudioEngine &audio)
{
    // images
    background_jungle_texture = CreateImage(engine, "backgrounds/PNG/Battleground4/Pale/Battleground4.png");

    LoadKnight(engine);
    LoadGhost(engine);
    LoadMinotaur(engine);
    LoadSkeleton(engine);
    LoadSatyr(engine);
    LoadVampire(engine);
    LoadWerewolf(engine);
    LoadSamurai(engine);
    LoadShinobi(engine);
    LoadWizard(engine);
    LoadSelectSound(audio);
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

void AssetManager::LoadSelectSound(olc::ext::Miniaudio::AudioEngine &audio)
{
    sfx_select = std::make_shared<olc::ext::Miniaudio::Sound>();
    audio.CreateSoundFromFile(*sfx_select, "assets/sfx/select.wav");
}

void AssetManager::LoadKnight(olc::PixelGameEngine &engine)
{
    const auto idle_texture = knight_idle_texture = CreateImage(engine, "knight/Knight_1/Idle.png");
    const auto walk_texture = knight_walk_texture = CreateImage(engine, "knight/Knight_1/Walk.png");
    const auto attack1_texture = knight_attack1_texture = CreateImage(engine, "knight/Knight_1/Attack 1.png");
    // animations
    auto idle_animation_defintion = CreateSimpleAnimation(
        idle_texture,
        "idle",
        {4, 1},
        {{"pivot", {0.25f, 1.0f}}},
        {.33f, .33f, .33f, .33f, .33f, .33f, .33f, .33f}
    );

    auto walk_animation_defintion = CreateSimpleAnimation(
        walk_texture,
        "walk",
        {8, 1},
        {{"pivot", {0.25f, 1.0f}}},
        {.33f, .33f, .33f, .33f, .33f, .33f, .33f, .33f, .33f, .33f, .33f, .33f, .33f, .33f, .33f, .33f}
    );

    auto attack1_animation_defintion = CreateSimpleAnimation(
        attack1_texture,
        "attack1",
        {5, 1},
        {{"pivot", {0.5f, 1.0f}}},
        {.33f, .33f, .33f, .33f, .33f, .33f, .33f, .33f, .33f, .33f, .33f, .33f, .33f, .33f, .33f, .33f}
    );

    // animators
    knight_animator = CreateAnimator({
        idle_animation_defintion,
        walk_animation_defintion,
        attack1_animation_defintion,
    });
}

void AssetManager::LoadMinotaur(olc::PixelGameEngine &engine)
{
    minotaur_idle_texture = CreateImage(engine, "minotaur/Minotaur_1/Idle.png");
    minotaur_walk_texture = CreateImage(engine, "minotaur/Minotaur_1/Walk.png");
    minotaur_attack1_texture = CreateImage(engine, "minotaur/Minotaur_1/Attack.png");
    // animations
    auto idle_animation_defintion = CreateSimpleAnimation(
        minotaur_idle_texture,
        "idle",
        {10, 1},
        {{"pivot", {0.5f, 1.0f}}},
        {.33f, .33f, .33f, .33f, .33f, .33f, .33f, .33f, .33f, .33f, .33f, .33f, .33f, .33f, .33f, .33f}
    );

    auto walk_animation_defintion = CreateSimpleAnimation(
        minotaur_walk_texture,
        "walk",
        {12, 1},
        {{"pivot", {0.5f, 1.0f}}},
        {.33f, .33f, .33f, .33f, .33f, .33f, .33f, .33f, .33f, .33f, .33f, .33f, .33f, .33f, .33f, .33f}
    );

    auto attack1_animation_defintion = CreateSimpleAnimation(
        minotaur_attack1_texture,
        "attack1",
        {5, 1},
        {{"pivot", {0.25f, 1.0f}}},
        {.33f, .33f, .33f, .33f, .33f, .33f, .33f, .33f, .33f, .33f, .33f, .33f, .33f, .33f, .33f, .33f}
    );
    // animators
    minotaur_animator = CreateAnimator({
        idle_animation_defintion,
        walk_animation_defintion,
        attack1_animation_defintion,
    });
}

void AssetManager::LoadSatyr(olc::PixelGameEngine &engine)
{
    satyr_idle_texture = CreateImage(engine, "satyr/Satyr_2/Idle.png");
    satyr_walk_texture = CreateImage(engine, "satyr/Satyr_2/Walk.png");
    satyr_attack1_texture = CreateImage(engine, "satyr/Satyr_2/Attack.png");
    // animations
    auto idle_animation_defintion = CreateSimpleAnimation(
        satyr_idle_texture,
        "idle",
        {7, 1},
        {{"pivot", {0.5f, 1.0f}}},
        {.33f, .33f, .33f, .33f, .33f, .33f, .33f, .33f, .33f, .33f, .33f, .33f, .33f, .33f, .33f, .33f}
    );

    auto walk_animation_defintion = CreateSimpleAnimation(
        satyr_walk_texture,
        "walk",
        {12, 1},
        {{"pivot", {0.5f, 1.0f}}},
        {.33f, .33f, .33f, .33f, .33f, .33f, .33f, .33f, .33f, .33f, .33f, .33f, .33f, .33f, .33f, .33f}
    );

    auto attack1_animation_defintion = CreateSimpleAnimation(
        satyr_attack1_texture,
        "attack1",
        {8, 1},
        {{"pivot", {0.5f, 1.0f}}},
        {.33f, .33f, .33f, .33f, .33f, .33f, .33f, .33f, .33f, .33f, .33f, .33f, .33f, .33f, .33f, .33f}
    );
    // animators
    satyr_animator = CreateAnimator({
        idle_animation_defintion,
        walk_animation_defintion,
        attack1_animation_defintion,
    });
}

void AssetManager::LoadSkeleton(olc::PixelGameEngine &engine)
{
    skeleton_idle_texture = CreateImage(engine, "skeleton/Skeleton_Warrior/Idle.png");
    skeleton_walk_texture = CreateImage(engine, "skeleton/Skeleton_Warrior/Walk.png");
    skeleton_attack1_texture = CreateImage(engine, "skeleton/Skeleton_Warrior/Attack_1.png");
    // animations
    auto idle_animation_defintion = CreateSimpleAnimation(
        skeleton_idle_texture,
        "idle",
        {7, 1},
        {{"pivot", {0.5f, 1.0f}}},
        {.33f, .33f, .33f, .33f, .33f, .33f, .33f, .33f, .33f, .33f, .33f, .33f, .33f, .33f, .33f, .33f}
    );

    auto walk_animation_defintion = CreateSimpleAnimation(
        skeleton_walk_texture,
        "walk",
        {7, 1},
        {{"pivot", {0.5f, 1.0f}}},
        {.33f, .33f, .33f, .33f, .33f, .33f, .33f, .33f, .33f, .33f, .33f, .33f, .33f, .33f, .33f, .33f}
    );

    auto attack1_animation_defintion = CreateSimpleAnimation(
        skeleton_attack1_texture,
        "attack1",
        {5, 1},
        {{"pivot", {0.5f, 1.0f}}},
        {.33f, .33f, .33f, .33f, .33f, .33f, .33f, .33f, .33f, .33f, .33f, .33f, .33f, .33f, .33f, .33f}
    );
    // animators
    skeleton_animator = CreateAnimator({
        idle_animation_defintion,
        walk_animation_defintion,
        attack1_animation_defintion,
    });
}

void AssetManager::LoadWerewolf(olc::PixelGameEngine &engine)
{
    werewolf_idle_texture = CreateImage(engine, "werewolf/Black_Werewolf/Idle.png");
    werewolf_walk_texture = CreateImage(engine, "werewolf/Black_Werewolf/walk.png");
    werewolf_attack1_texture = CreateImage(engine, "werewolf/Black_Werewolf/Attack_1.png");
    // animations
    auto idle_animation_defintion = CreateSimpleAnimation(
        werewolf_idle_texture,
        "idle",
        {8, 1},
        {{"pivot", {0.5f, 1.0f}}},
        {.33f, .33f, .33f, .33f, .33f, .33f, .33f, .33f, .33f, .33f, .33f, .33f, .33f, .33f, .33f, .33f}
    );

    auto walk_animation_defintion = CreateSimpleAnimation(
        werewolf_walk_texture,
        "walk",
        {11, 1},
        {{"pivot", {0.5f, 1.0f}}},
        {.33f, .33f, .33f, .33f, .33f, .33f, .33f, .33f, .33f, .33f, .33f, .33f, .33f, .33f, .33f, .33f}
    );

    auto attack1_animation_defintion = CreateSimpleAnimation(
        werewolf_attack1_texture,
        "attack1",
        {6, 1},
        {{"pivot", {0.5f, 1.0f}}},
        {.33f, .33f, .33f, .33f, .33f, .33f, .33f, .33f, .33f, .33f, .33f, .33f, .33f, .33f, .33f, .33f}
    );
    // animators
    werewolf_animator = CreateAnimator({
        idle_animation_defintion,
        walk_animation_defintion,
        attack1_animation_defintion,
    });
}

void AssetManager::LoadGhost(olc::PixelGameEngine &engine)
{
    ghost_idle_texture = CreateImage(engine, "ghost/Yurei/Idle.png");
    ghost_walk_texture = CreateImage(engine, "ghost/Yurei/Walk.png");
    ghost_attack1_texture = CreateImage(engine, "ghost/Yurei/Attack_1.png");
    // animations
    auto idle_animation_defintion = CreateSimpleAnimation(
        ghost_idle_texture,
        "idle",
        {5, 1},
        {{"pivot", {0.5f, 1.0f}}},
        {.33f, .33f, .33f, .33f, .33f, .33f, .33f, .33f, .33f, .33f, .33f, .33f, .33f, .33f, .33f, .33f}
    );

    auto walk_animation_defintion = CreateSimpleAnimation(
        ghost_walk_texture,
        "walk",
        {5, 1},
        {{"pivot", {0.5f, 1.0f}}},
        {.33f, .33f, .33f, .33f, .33f, .33f, .33f, .33f, .33f, .33f, .33f, .33f, .33f, .33f, .33f, .33f}
    );

    auto attack1_animation_defintion = CreateSimpleAnimation(
        ghost_attack1_texture,
        "attack1",
        {4, 1},
        {{"pivot", {0.5f, 1.0f}}},
        {.33f, .33f, .33f, .33f, .33f, .33f, .33f, .33f, .33f, .33f, .33f, .33f, .33f, .33f, .33f, .33f}
    );
    // animators
    ghost_animator = CreateAnimator({
        idle_animation_defintion,
        walk_animation_defintion,
        attack1_animation_defintion,
    });
}

void AssetManager::LoadVampire(olc::PixelGameEngine &engine)
{
    vampire_idle_texture = CreateImage(engine, "vampire/Countess_Vampire/Idle.png");
    vampire_walk_texture = CreateImage(engine, "vampire/Countess_Vampire/Walk.png");
    vampire_attack1_texture = CreateImage(engine, "vampire/Countess_Vampire/Attack_1.png");
    // animations
    auto idle_animation_defintion = CreateSimpleAnimation(
        vampire_idle_texture,
        "idle",
        {5, 1},
        {{"pivot", {0.5f, 1.0f}}},
        {.33f, .33f, .33f, .33f, .33f, .33f, .33f, .33f, .33f, .33f, .33f, .33f, .33f, .33f, .33f, .33f}
    );

    auto walk_animation_defintion = CreateSimpleAnimation(
        vampire_walk_texture,
        "walk",
        {6, 1},
        {{"pivot", {0.5f, 1.0f}}},
        {.33f, .33f, .33f, .33f, .33f, .33f, .33f, .33f, .33f, .33f, .33f, .33f, .33f, .33f, .33f, .33f}
    );

    auto attack1_animation_defintion = CreateSimpleAnimation(
        vampire_attack1_texture,
        "attack1",
        {6, 1},
        {{"pivot", {0.5f, 1.0f}}},
        {.33f, .33f, .33f, .33f, .33f, .33f, .33f, .33f, .33f, .33f, .33f, .33f, .33f, .33f, .33f, .33f}
    );
    // animators
    vampire_animator = CreateAnimator({
        idle_animation_defintion,
        walk_animation_defintion,
        attack1_animation_defintion,
    });
}

void AssetManager::LoadShinobi(olc::PixelGameEngine &engine)
{
    const auto idle_texture = shinobi_idle_texture = CreateImage(engine, "shinobi/Shinobi/Idle.png");
    const auto walk_texture = shinobi_walk_texture = CreateImage(engine, "shinobi/Shinobi/Walk.png");
    const auto attack1_texture = shinobi_attack1_texture = CreateImage(engine, "shinobi/Shinobi/Attack_1.png");
    // animations
    auto idle_animation_defintion = CreateSimpleAnimation(
        idle_texture,
        "idle",
        {6, 1},
        {{"pivot", {0.5f, 1.0f}}},
        {.33f, .33f, .33f, .33f, .33f, .33f, .33f, .33f, .33f, .33f, .33f, .33f, .33f, .33f, .33f, .33f}
    );

    auto walk_animation_defintion = CreateSimpleAnimation(
        walk_texture,
        "walk",
        {8, 1},
        {{"pivot", {0.5f, 1.0f}}},
        {.33f, .33f, .33f, .33f, .33f, .33f, .33f, .33f, .33f, .33f, .33f, .33f, .33f, .33f, .33f, .33f}
    );

    auto attack1_animation_defintion = CreateSimpleAnimation(
        attack1_texture,
        "attack1",
        {5, 1},
        {{"pivot", {0.5f, 1.0f}}},
        {.33f, .33f, .33f, .33f, .33f, .33f, .33f, .33f, .33f, .33f, .33f, .33f, .33f, .33f, .33f, .33f}
    );
    // animators
    shinobi_animator = CreateAnimator({
        idle_animation_defintion,
        walk_animation_defintion,
        attack1_animation_defintion,
    });
}

void AssetManager::LoadSamurai(olc::PixelGameEngine &engine)
{
    const auto &idle_texture = shinobi_idle_texture = CreateImage(engine, "samurai/Samurai/Idle.png");
    const auto &walk_texture = shinobi_walk_texture = CreateImage(engine, "samurai/Samurai/Walk.png");
    const auto &attack1_texture = shinobi_attack1_texture = CreateImage(engine, "samurai/Samurai/Attack_1.png");
    // animations
    auto idle_animation_defintion = CreateSimpleAnimation(
        idle_texture,
        "idle",
        {6, 1},
        {{"pivot", {0.25f, 1.0f}}},
        {.33f, .33f, .33f, .33f, .33f, .33f, .33f, .33f, .33f, .33f, .33f, .33f, .33f, .33f, .33f, .33f}
    );

    auto walk_animation_defintion = CreateSimpleAnimation(
        walk_texture,
        "walk",
        {9, 1},
        {{"pivot", {0.5f, 1.0f}}},
        {.33f, .33f, .33f, .33f, .33f, .33f, .33f, .33f, .33f, .33f, .33f, .33f, .33f, .33f, .33f, .33f}
    );

    auto attack1_animation_defintion = CreateSimpleAnimation(
        attack1_texture,
        "attack1",
        {4, 1},
        {{"pivot", {0.5f, 1.0f}}},
        {.33f, .33f, .33f, .33f, .33f, .33f, .33f, .33f, .33f, .33f, .33f, .33f, .33f, .33f, .33f, .33f}
    );
    // animators
    samurai_animator = CreateAnimator({
        idle_animation_defintion,
        walk_animation_defintion,
        attack1_animation_defintion,
    });
}

void AssetManager::LoadWizard(olc::PixelGameEngine &engine)
{
    const auto &idle_texture = shinobi_idle_texture = CreateImage(engine, "wizard/Wanderer Magican/Idle.png");
    const auto &walk_texture = shinobi_walk_texture = CreateImage(engine, "wizard/Wanderer Magican/Walk.png");
    const auto &attack1_texture = shinobi_attack1_texture = CreateImage(engine, "wizard/Wanderer Magican/Attack_1.png");
    // animations
    auto idle_animation_defintion = CreateSimpleAnimation(
        idle_texture,
        "idle",
        {8, 1},
        {{"pivot", {0.5f, 1.0f}}},
        {.33f, .33f, .33f, .33f, .33f, .33f, .33f, .33f, .33f, .33f, .33f, .33f, .33f, .33f, .33f, .33f}
    );

    auto walk_animation_defintion = CreateSimpleAnimation(
        walk_texture,
        "walk",
        {7, 1},
        {{"pivot", {0.5f, 1.0f}}},
        {.33f, .33f, .33f, .33f, .33f, .33f, .33f, .33f, .33f, .33f, .33f, .33f, .33f, .33f, .33f, .33f}
    );

    auto attack1_animation_defintion = CreateSimpleAnimation(
        attack1_texture,
        "attack1",
        {7, 1},
        {{"pivot", {0.25f, 1.0f}}},
        {.33f, .33f, .33f, .33f, .33f, .33f, .33f, .33f, .33f, .33f, .33f, .33f, .33f, .33f, .33f, .33f}
    );
    // animators
    wizard_animator = CreateAnimator({
        idle_animation_defintion,
        walk_animation_defintion,
        attack1_animation_defintion,
    });
}