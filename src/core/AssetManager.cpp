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
    LoadSatyrMissle1(engine);
    LoadVampireMissle1(engine);
    LoadWizardMissle1(engine);
    LoadSwarpSound(audio);
    LoadKnightAttackSound(audio);
    LoadShinobiAttackSound(audio);
    LoadSamuraiAttackSound(audio);
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

void AssetManager::LoadSwarpSound(olc::ext::Miniaudio::AudioEngine &audio)
{
    sfx_swarp = std::make_shared<olc::ext::Miniaudio::Sound>();
    audio.CreateSoundFromFile(*sfx_swarp, "assets/sfx/swarp2.wav");
}

void AssetManager::LoadKnightAttackSound(olc::ext::Miniaudio::AudioEngine &audio)
{
    sfx_knight_attack = std::make_shared<olc::ext::Miniaudio::Sound>();
    audio.CreateSoundFromFile(*sfx_knight_attack, "assets/sfx/knight_attack.wav");
}

void AssetManager::LoadSamuraiAttackSound(olc::ext::Miniaudio::AudioEngine &audio)
{
    sfx_samurai_attack = std::make_shared<olc::ext::Miniaudio::Sound>();
    audio.CreateSoundFromFile(*sfx_samurai_attack, "assets/sfx/samurai_attack.wav");
}

void AssetManager::LoadShinobiAttackSound(olc::ext::Miniaudio::AudioEngine &audio)
{
    sfx_shinobi_attack = std::make_shared<olc::ext::Miniaudio::Sound>();
    audio.CreateSoundFromFile(*sfx_shinobi_attack, "assets/sfx/shinobi_attack.wav");
}

void AssetManager::LoadKnight(olc::PixelGameEngine &engine)
{
    const auto &idle_texture = knight_idle_texture = CreateImage(engine, "knight/Knight_1/Idle.png");
    const auto &walk_texture = knight_walk_texture = CreateImage(engine, "knight/Knight_1/Walk.png");
    const auto &attack1_texture = knight_attack1_texture = CreateImage(engine, "knight/Knight_1/Attack 1.png");
    const auto &die_texture = knight_die_texture = CreateImage(engine, "knight/Knight_1/Dead.png");
    // animations
    auto idle_animation_definition = CreateSimpleAnimation(
        idle_texture,
        "idle",
        {4, 1},
        {{"pivot", {0.25f, 1.0f}}, {"body", {0.5f, 0.5f}}},
        {.33f, .33f, .33f, .33f, .33f, .33f, .33f, .33f}
    );

    auto walk_animation_defintion = CreateSimpleAnimation(
        walk_texture,
        "walk",
        {8, 1},
        {{"pivot", {0.25f, 1.0f}}, {"body", {0.5f, 0.5f}}},
        get_default_animation_times()
    );

    auto attack1_animation_definition = CreateSimpleAnimation(
        attack1_texture,
        "attack1",
        {5, 1},
        {{"pivot", {0.5f, 1.0f}}, {"body", {0.5f, 0.5f}}},
        get_default_animation_times()
    );

    auto die_animation_definition = CreateSimpleAnimation(
        die_texture,
        "die",
        {6, 1},
        {{"pivot", {0.25f, 1.0f}}, {"body", {0.5f, 0.5f}}},
        get_default_animation_times()
    );

    // animators
    knight_animator = CreateAnimator({
        idle_animation_definition,
        walk_animation_defintion,
        attack1_animation_definition,
        die_animation_definition,
    });
}

void AssetManager::LoadMinotaur(olc::PixelGameEngine &engine)
{
    const auto &idle_texture = minotaur_idle_texture = CreateImage(engine, "minotaur/Minotaur_1/Idle.png");
    const auto &walk_texture = minotaur_walk_texture = CreateImage(engine, "minotaur/Minotaur_1/Walk.png");
    const auto &attack1_texture = minotaur_attack1_texture = CreateImage(engine, "minotaur/Minotaur_1/Attack.png");
    const auto &die_texture = minotaur_die_texture = CreateImage(engine, "minotaur/Minotaur_1/Dead.png");
    // animations
    auto idle_animation_definition = CreateSimpleAnimation(
        minotaur_idle_texture,
        "idle",
        {10, 1},
        {{"pivot", {0.4f, 1.0f}}},
        get_default_animation_times()
    );

    auto walk_animation_defintion = CreateSimpleAnimation(
        minotaur_walk_texture,
        "walk",
        {12, 1},
        {{"pivot", {0.5f, 1.0f}}},
        get_default_animation_times()
    );

    auto attack1_animation_definition = CreateSimpleAnimation(
        minotaur_attack1_texture,
        "attack1",
        {5, 1},
        {{"pivot", {0.3f, 1.0f}}},
        get_default_animation_times()
    );

    auto die_animation_definition = CreateSimpleAnimation(
        die_texture,
        "die",
        {5, 1},
        {{"pivot", {0.25f, 1.0f}}},
        {
            0.5f,
            0.125f,
            0.125f,
            0.125f,
            0.125f,
        }
    );
    // animators
    minotaur_animator = CreateAnimator({
        idle_animation_definition,
        walk_animation_defintion,
        attack1_animation_definition,
        die_animation_definition,
    });
}

void AssetManager::LoadSatyr(olc::PixelGameEngine &engine)
{
    const auto &idle_texture = satyr_idle_texture = CreateImage(engine, "satyr/Satyr_2/Idle.png");
    const auto &walk_texture = satyr_walk_texture = CreateImage(engine, "satyr/Satyr_2/Walk.png");
    const auto &attack1_texture = satyr_attack1_texture = CreateImage(engine, "satyr/Satyr_2/Attack.png");
    const auto &missle1_texture = satyr_missle1_texture = CreateImage(engine, "satyr/Satyr_2/Charge.png");
    const auto &die_texture = satyr_die_texture = CreateImage(engine, "satyr/Satyr_2/Dead.png");
    // animations
    auto idle_animation_definition =
        CreateSimpleAnimation(idle_texture, "idle", {7, 1}, {{"pivot", {0.5f, 1.0f}}}, get_default_animation_times());

    auto walk_animation_defintion =
        CreateSimpleAnimation(walk_texture, "walk", {12, 1}, {{"pivot", {0.5f, 1.0f}}}, get_default_animation_times());

    auto attack1_animation_definition = CreateSimpleAnimation(
        attack1_texture,
        "attack1",
        {8, 1},
        {{"pivot", {0.5f, 1.0f}}, {"weapon", {0.7f, 0.425f}}},
        get_default_animation_times()
    );

    auto die_animation_definition =
        CreateSimpleAnimation(die_texture, "die", {4, 1}, {{"pivot", {0.5f, 1.0f}}}, {1.f, 1.0f, 1.5f, 4.0f});
    // animators
    satyr_animator = CreateAnimator({
        idle_animation_definition,
        walk_animation_defintion,
        attack1_animation_definition,
        die_animation_definition,
    });
}

void AssetManager::LoadSatyrMissle1(olc::PixelGameEngine &engine)
{
    const auto &texture = satyr_missle1_texture = CreateImage(engine, "satyr/Satyr_2/Charge.png");
    // animations
    auto idle_animation_definition = AnimationDefinition{
        .name = "idle",
        .image = texture,
        .frame_slices = {8, 1},
        .frames = {
            {
                .slice_index = {1, 0},
                .seconds = 0.5f,
                .anchors = {{"pivot", {0.5f, 0.5f}}},
            },
            {
                .slice_index = {2, 0},
                .seconds = 0.5f,
                .anchors = {{"pivot", {0.5f, 0.5f}}},
            },
        },
    };

    auto walk_animation_definition = AnimationDefinition{
        .name = "walk",
        .image = texture,
        .frame_slices = {8, 1},
        .frames = {
            {
                .slice_index = {1, 0},
                .seconds = 0.16f,
                .anchors = {{"pivot", {0.5f, 0.5f}}},
            },
            {
                .slice_index = {2, 0},
                .seconds = 0.16f,
                .anchors = {{"pivot", {0.5f, 0.5f}}},
            },
        },
    };

    auto die_animation_definition = AnimationDefinition{
        .name = "die",
        .image = texture,
        .frame_slices = {8, 1},
        .frames = {
            {.slice_index = {1, 0}, .seconds = 0.16f, .anchors = {{"pivot", {0.5f, 0.5f}}}},
            {.slice_index = {2, 0}, .seconds = 0.16f, .anchors = {{"pivot", {0.5f, 0.5f}}}},
            {.slice_index = {3, 0}, .seconds = 0.16f, .anchors = {{"pivot", {0.5f, 0.5f}}}},
            {.slice_index = {4, 0}, .seconds = 0.16f, .anchors = {{"pivot", {0.5f, 0.5f}}}},
            {.slice_index = {5, 0}, .seconds = 0.16f, .anchors = {{"pivot", {0.5f, 0.5f}}}},
            {.slice_index = {6, 0}, .seconds = 0.33f, .anchors = {{"pivot", {0.5f, 0.5f}}}},
            {.slice_index = {7, 0}, .seconds = 0.5f, .anchors = {{"pivot", {0.5f, 0.5f}}}},
        },
    };

    // animators
    satyr_missle1_animator = CreateAnimator({
        idle_animation_definition,
        walk_animation_definition,
        die_animation_definition,
    });
}

void AssetManager::LoadSkeleton(olc::PixelGameEngine &engine)
{
    const auto &idle_texture = skeleton_idle_texture = CreateImage(engine, "skeleton/Skeleton_Warrior/Idle.png");
    const auto &walk_texture = skeleton_walk_texture = CreateImage(engine, "skeleton/Skeleton_Warrior/Walk.png");
    const auto &attack1_texture = skeleton_attack1_texture =
        CreateImage(engine, "skeleton/Skeleton_Warrior/Attack_1.png");
    const auto &attack2_texture = skeleton_attack2_texture =
        CreateImage(engine, "skeleton/Skeleton_Warrior/Attack_2.png");
    const auto &attack3_texture = skeleton_attack3_texture =
        CreateImage(engine, "skeleton/Skeleton_Warrior/Attack_3.png");
    const auto &die_texture = skeleton_die_texture = CreateImage(engine, "skeleton/Skeleton_Warrior/Dead.png");
    // animations
    auto idle_animation_definition = CreateSimpleAnimation(
        skeleton_idle_texture,
        "idle",
        {7, 1},
        {{"pivot", {0.5f, 1.0f}}},
        get_default_animation_times()
    );

    auto walk_animation_defintion = CreateSimpleAnimation(
        skeleton_walk_texture,
        "walk",
        {7, 1},
        {{"pivot", {0.5f, 1.0f}}},
        get_default_animation_times()
    );

    auto attack1_animation_definition = CreateSimpleAnimation(
        skeleton_attack1_texture,
        "attack1",
        {5, 1},
        {{"pivot", {0.5f, 1.0f}}},
        {0.3f, 0.15f, 0.15f, 0.15f, 0.3f}
    );

    auto attack2_animation_defintion = CreateSimpleAnimation(
        skeleton_attack2_texture,
        "attack2",
        {6, 1},
        {{"pivot", {0.5f, 1.0f}}},
        {0.1f, 0.1f, 0.1f, 0.25f, 0.1f, 0.1f}
    );

    auto attack3_animation_defintion = CreateSimpleAnimation(
        skeleton_attack3_texture,
        "attack3",
        {4, 1},
        {{"pivot", {0.5f, 1.0f}}},
        {0.2f, 0.2f, 0.2f, 0.2f}
    );

    auto die_animation_definition =
        CreateSimpleAnimation(die_texture, "die", {4, 1}, {{"pivot", {0.5f, 1.0f}}}, {0.5, 1.0f, 2.0f, 6.0f});
    // animators
    skeleton_animator = CreateAnimator({
        idle_animation_definition,
        walk_animation_defintion,
        attack1_animation_definition,
        attack2_animation_defintion,
        attack3_animation_defintion,
        die_animation_definition,
    });
}

void AssetManager::LoadWerewolf(olc::PixelGameEngine &engine)
{
    const auto &idle_texture = werewolf_idle_texture = CreateImage(engine, "werewolf/Black_Werewolf/Idle.png");
    const auto &walk_texture = werewolf_walk_texture = CreateImage(engine, "werewolf/Black_Werewolf/walk.png");
    const auto &attack1_texture = werewolf_attack1_texture =
        CreateImage(engine, "werewolf/Black_Werewolf/Attack_1.png");
    const auto &attack2_texture = werewolf_attack2_texture =
        CreateImage(engine, "werewolf/Black_Werewolf/Attack_2.png");
    const auto &attack3_texture = werewolf_attack3_texture =
        CreateImage(engine, "werewolf/Black_Werewolf/Attack_3.png");
    const auto &die_texture = werewolf_die_texture = CreateImage(engine, "werewolf/Black_Werewolf/Dead.png");
    // animations
    auto idle_animation_definition = CreateSimpleAnimation(
        werewolf_idle_texture,
        "idle",
        {8, 1},
        {{"pivot", {0.5f, 1.0f}}},
        get_default_animation_times()
    );

    auto walk_animation_defintion = CreateSimpleAnimation(
        werewolf_walk_texture,
        "walk",
        {11, 1},
        {{"pivot", {0.5f, 1.0f}}},
        get_default_animation_times()
    );

    auto attack1_animation_definition = CreateSimpleAnimation(
        werewolf_attack1_texture,
        "attack1",
        {6, 1},
        {{"pivot", {0.5f, 1.0f}}},
        get_default_animation_times()
    );
    auto attack2_animation_definition = CreateSimpleAnimation(
        werewolf_attack2_texture,
        "attack2",
        {4, 1},
        {{"pivot", {0.5f, 1.0f}}},
        get_default_animation_times()
    );
    auto attack3_animation_definition = CreateSimpleAnimation(
        werewolf_attack3_texture,
        "attack3",
        {5, 1},
        {{"pivot", {0.5f, 1.0f}}},
        get_default_animation_times()
    );

    auto die_animation_definition =
        CreateSimpleAnimation(die_texture, "die", {2, 1}, {{"pivot", {0.25f, 1.0f}}}, {1.25f, 8.0f});
    // animators
    werewolf_animator = CreateAnimator({
        idle_animation_definition,
        walk_animation_defintion,
        attack1_animation_definition,
        attack2_animation_definition,
        attack3_animation_definition,
        die_animation_definition,
    });
}

void AssetManager::LoadGhost(olc::PixelGameEngine &engine)
{
    const auto &idle_texture = ghost_idle_texture = CreateImage(engine, "ghost/Yurei/Idle.png");
    const auto &walk_texture = ghost_walk_texture = CreateImage(engine, "ghost/Yurei/Walk.png");
    const auto &attack1_texture = ghost_attack1_texture = CreateImage(engine, "ghost/Yurei/Attack_1.png");
    const auto &die_texture = ghost_die_texture = CreateImage(engine, "ghost/Yurei/Dead.png");
    // animations
    auto idle_animation_definition = CreateSimpleAnimation(
        ghost_idle_texture,
        "idle",
        {5, 1},
        {{"pivot", {0.5f, 1.0f}}},
        get_default_animation_times()
    );

    auto walk_animation_defintion = CreateSimpleAnimation(
        ghost_walk_texture,
        "walk",
        {5, 1},
        {{"pivot", {0.5f, 1.0f}}},
        get_default_animation_times()
    );

    auto attack1_animation_definition = CreateSimpleAnimation(
        ghost_attack1_texture,
        "attack1",
        {4, 1},
        {{"pivot", {0.5f, 1.0f}}},
        get_default_animation_times()
    );

    auto die_animation_definition =
        CreateSimpleAnimation(die_texture, "die", {4, 1}, {{"pivot", {0.5f, 1.0f}}}, get_default_animation_times());
    // animators
    ghost_animator = CreateAnimator({
        idle_animation_definition,
        walk_animation_defintion,
        attack1_animation_definition,
        die_animation_definition,
    });
}

void AssetManager::LoadVampire(olc::PixelGameEngine &engine)
{
    const auto &idle_texture = vampire_idle_texture = CreateImage(engine, "vampire/Countess_Vampire/Idle.png");
    const auto &walk_texture = vampire_walk_texture = CreateImage(engine, "vampire/Countess_Vampire/Walk.png");
    const auto &attack1_texture = vampire_attack1_texture =
        CreateImage(engine, "vampire/Countess_Vampire/Attack_1.png");
    const auto &die_texture = vampire_die_texture = CreateImage(engine, "vampire/Countess_Vampire/Dead.png");
    // animations
    auto idle_animation_definition = CreateSimpleAnimation(
        vampire_idle_texture,
        "idle",
        {5, 1},
        {{"pivot", {0.5f, 1.0f}}, {"body", {0.5f, 0.5f}}, {"body", {0.5f, 0.5f}}},
        get_default_animation_times()
    );

    auto walk_animation_defintion = CreateSimpleAnimation(
        vampire_walk_texture,
        "walk",
        {6, 1},
        {{"pivot", {0.5f, 1.0f}}, {"body", {0.5f, 0.5f}}},
        get_default_animation_times()
    );

    auto attack1_animation_definition = CreateSimpleAnimation(
        vampire_attack1_texture,
        "attack1",
        {6, 1},
        {{"pivot", {0.5f, 1.0f}}, {"weapon", {0.7f, 0.5f}}, {"body", {0.5f, 0.5f}}},
        get_default_animation_times()
    );

    auto die_animation_definition =
        CreateSimpleAnimation(die_texture, "die", {8, 1}, {{"pivot", {0.5f, 1.0f}}}, get_default_animation_times());
    // animators
    vampire_animator = CreateAnimator({
        idle_animation_definition,
        walk_animation_defintion,
        attack1_animation_definition,
        die_animation_definition,
    });
}

void AssetManager::LoadVampireMissle1(olc::PixelGameEngine &engine)
{
    const auto &texture = vampire_missle1_texture = CreateImage(engine, "vampire/Countess_Vampire/Blood_Charge_4.png");
    // animations
    auto idle_animation_definition = AnimationDefinition{
        .name = "idle",
        .image = texture,
        .frame_slices = {4, 1},
        .frames = {{
            .slice_index = {1, 0},
            .seconds = 0.5f,
            .anchors = {{"pivot", {0.5f, 0.5f}}},
        }},
    };

    auto walk_animation_definition = AnimationDefinition{
        .name = "walk",
        .image = texture,
        .frame_slices = {4, 1},
        .frames = {
            {.slice_index = {1, 0}, .seconds = 0.16f, .anchors = {{"pivot", {0.5f, 0.5f}}}},
            {.slice_index = {2, 0}, .seconds = 0.16f, .anchors = {{"pivot", {0.5f, 0.5f}}}},
            {.slice_index = {3, 0}, .seconds = 0.16f, .anchors = {{"pivot", {0.5f, 0.5f}}}},
            {.slice_index = {4, 0}, .seconds = 0.16f, .anchors = {{"pivot", {0.5f, 0.5f}}}},
        },
    };

    auto die_animation_definition = AnimationDefinition{
        .name = "die",
        .image = texture,
        .frame_slices = {4, 1},
        .frames = {
            {.slice_index = {1, 0}, .seconds = 0.16f, .anchors = {{"pivot", {0.5f, 0.5f}}}},
            {.slice_index = {2, 0}, .seconds = 0.16f, .anchors = {{"pivot", {0.5f, 0.5f}}}},
            {.slice_index = {3, 0}, .seconds = 0.16f, .anchors = {{"pivot", {0.5f, 0.5f}}}},
            {.slice_index = {4, 0}, .seconds = 0.16f, .anchors = {{"pivot", {0.5f, 0.5f}}}},
        },
    };

    // animators
    vampire_missle1_animator = CreateAnimator({
        idle_animation_definition,
        walk_animation_definition,
        die_animation_definition,
    });
}

void AssetManager::LoadShinobi(olc::PixelGameEngine &engine)
{
    const auto &idle_texture = shinobi_idle_texture = CreateImage(engine, "shinobi/Shinobi/Idle.png");
    const auto &walk_texture = shinobi_walk_texture = CreateImage(engine, "shinobi/Shinobi/Walk.png");
    const auto &attack1_texture = shinobi_attack1_texture = CreateImage(engine, "shinobi/Shinobi/Attack_1.png");
    const auto &attack2_texture = shinobi_attack2_texture = CreateImage(engine, "shinobi/Shinobi/Attack_2.png");
    const auto &attack3_texture = shinobi_attack3_texture = CreateImage(engine, "shinobi/Shinobi/Attack_3.png");
    const auto &hurt_texture = shinobi_hurt_texture = CreateImage(engine, "shinobi/Shinobi/Hurt.png");
    const auto &die_texture = shinobi_die_texture = CreateImage(engine, "shinobi/Shinobi/Dead.png");
    // animations
    auto idle_animation_definition =
        CreateSimpleAnimation(idle_texture, "idle", {6, 1}, {{"pivot", {0.5f, 1.0f}}}, get_default_animation_times());

    auto walk_animation_defintion =
        CreateSimpleAnimation(walk_texture, "walk", {8, 1}, {{"pivot", {0.5f, 1.0f}}}, get_default_animation_times());

    auto attack1_animation_definition = CreateSimpleAnimation(
        attack1_texture,
        "attack1",
        {5, 1},
        {{"pivot", {0.5f, 1.0f}}},
        get_default_animation_times()
    );
    auto attack2_animation_definition = CreateSimpleAnimation(
        attack2_texture,
        "attack2",
        {3, 1},
        {{"pivot", {0.5f, 1.0f}}},
        get_default_animation_times()
    );
    auto attack3_animation_definition = CreateSimpleAnimation(
        attack3_texture,
        "attack3",
        {4, 1},
        {{"pivot", {0.5f, 1.0f}}},
        get_default_animation_times()
    );

    auto hurt_animation_definition =
        CreateSimpleAnimation(hurt_texture, "hurt", {2, 1}, {{"pivot", {0.5f, 1.0f}}}, get_default_animation_times());

    auto die_animation_definition =
        CreateSimpleAnimation(die_texture, "die", {4, 1}, {{"pivot", {0.5f, 1.0f}}}, get_default_animation_times());
    // animators
    shinobi_animator = CreateAnimator({
        idle_animation_definition,
        walk_animation_defintion,
        attack1_animation_definition,
        attack2_animation_definition,
        attack3_animation_definition,
        hurt_animation_definition,
        die_animation_definition,
    });
}

void AssetManager::LoadSamurai(olc::PixelGameEngine &engine)
{
    const auto &idle_texture = samurai_idle_texture = CreateImage(engine, "samurai/Samurai/Idle.png");
    const auto &walk_texture = samurai_walk_texture = CreateImage(engine, "samurai/Samurai/Walk.png");
    const auto &attack1_texture = samurai_attack1_texture = CreateImage(engine, "samurai/Samurai/Attack_1.png");
    const auto &attack2_texture = samurai_attack2_texture = CreateImage(engine, "samurai/Samurai/Attack_2.png");
    const auto &attack3_texture = samurai_attack3_texture = CreateImage(engine, "samurai/Samurai/Attack_3.png");
    const auto &die_texture = samurai_die_texture = CreateImage(engine, "samurai/Samurai/Dead.png");
    // animations
    auto idle_animation_definition =
        CreateSimpleAnimation(idle_texture, "idle", {6, 1}, {{"pivot", {0.25f, 1.0f}}}, get_default_animation_times());

    auto walk_animation_defintion =
        CreateSimpleAnimation(walk_texture, "walk", {9, 1}, {{"pivot", {0.5f, 1.0f}}}, get_default_animation_times());

    auto attack1_animation_definition = CreateSimpleAnimation(
        attack1_texture,
        "attack1",
        {4, 1},
        {{"pivot", {0.5f, 1.0f}}},
        get_default_animation_times()
    );

    auto attack2_animation_definition = CreateSimpleAnimation(
        attack2_texture,
        "attack2",
        {5, 1},
        {{"pivot", {0.5f, 1.0f}}},
        get_default_animation_times()
    );

    auto attack3_animation_definition = CreateSimpleAnimation(
        attack3_texture,
        "attack3",
        {4, 1},
        {{"pivot", {0.5f, 1.0f}}},
        get_default_animation_times()
    );

    auto die_animation_definition =
        CreateSimpleAnimation(die_texture, "die", {6, 1}, {{"pivot", {0.5f, 1.0f}}}, get_default_animation_times());

    // animators
    samurai_animator = CreateAnimator({
        idle_animation_definition,
        walk_animation_defintion,
        attack1_animation_definition,
        attack2_animation_definition,
        attack3_animation_definition,
        die_animation_definition,
    });
}

void AssetManager::LoadWizard(olc::PixelGameEngine &engine)
{
    const auto &idle_texture = wizard_idle_texture = CreateImage(engine, "wizard/Wanderer Magican/Idle.png");
    const auto &walk_texture = wizard_walk_texture = CreateImage(engine, "wizard/Wanderer Magican/Walk.png");
    const auto &attack1_texture = wizard_attack1_texture = CreateImage(engine, "wizard/Wanderer Magican/Attack_1.png");
    const auto &cast1_texture = wizard_cast1_texture = CreateImage(engine, "wizard/Wanderer Magican/Magic_sphere.png");
    const auto &die_texture = wizard_die_texture = CreateImage(engine, "wizard/Wanderer Magican/Dead.png");
    // animations
    auto idle_animation_definition = CreateSimpleAnimation(
        idle_texture,
        "idle",
        {8, 1},
        {{"pivot", {0.5f, 1.0f}}, {"body", {0.5f, 0.5f}}},
        get_default_animation_times()
    );

    auto walk_animation_defintion = CreateSimpleAnimation(
        walk_texture,
        "walk",
        {7, 1},
        {{"pivot", {0.5f, 1.0f}}, {"body", {0.5f, 0.5f}}},
        get_default_animation_times()
    );

    auto cast1_animation_definition = CreateSimpleAnimation(
        cast1_texture,
        "cast1",
        {16, 1},
        {{"pivot", {0.33f, 1.0f}}, {"body", {0.5f, 0.5f}}, {"weapon", {0.7f, 0.7f}}},
        {
            0.33f,
            0.33f,
            0.33f,
            0.33f,
            0.33f,
            0.33f,
            0.33f,
            0.33f,
            0.33f,
            0.33f,
            0.33f,
            0.33f,
            0.33f,
            0.33f,
            0.33f,
            0.33f,
        }
    );

    auto attack1_animation_definition = CreateSimpleAnimation(
        attack1_texture,
        "attack1",
        {7, 1},
        {{"pivot", {0.25f, 1.0f}}, {"body", {0.5f, 0.5f}}},
        get_default_animation_times()
    );

    auto die_animation_definition = CreateSimpleAnimation(
        die_texture,
        "die",
        {4, 1},
        {{"pivot", {0.25f, 1.0f}}, {"body", {0.5f, 0.5f}}},
        get_default_animation_times()
    );

    // animators
    wizard_animator = CreateAnimator({
        idle_animation_definition,
        walk_animation_defintion,
        attack1_animation_definition,
        cast1_animation_definition,
        die_animation_definition,
    });
}

void AssetManager::LoadWizardMissle1(olc::PixelGameEngine &engine)
{
    const auto &texture = wizard_missle1_texture = CreateImage(engine, "wizard/Wanderer Magican/Charge_1.png");
    // animations
    auto idle_animation_definition = AnimationDefinition{
        .name = "idle",
        .image = texture,
        .frame_slices = {9, 1},
        .frames = {
            {
                .slice_index = {0, 0},
                .seconds = 0.5f,
                .anchors = {{"pivot", {0.5f, 0.5f}}},
            },
            {
                .slice_index = {1, 0},
                .seconds = 0.5f,
                .anchors = {{"pivot", {0.5f, 0.5f}}},
            },
        },
    };

    auto walk_animation_definition = AnimationDefinition{
        .name = "walk",
        .image = texture,
        .frame_slices = {9, 1},
        .frames = {
            {.slice_index = {0, 0}, .seconds = 0.16f, .anchors = {{"pivot", {0.5f, 0.5f}}}},
            {.slice_index = {1, 0}, .seconds = 0.16f, .anchors = {{"pivot", {0.5f, 0.5f}}}},
            {.slice_index = {2, 0}, .seconds = 0.16f, .anchors = {{"pivot", {0.5f, 0.5f}}}},
            {.slice_index = {3, 0}, .seconds = 0.16f, .anchors = {{"pivot", {0.5f, 0.5f}}}},
        },
    };

    auto die_animation_definition = AnimationDefinition{
        .name = "die",
        .image = texture,
        .frame_slices = {9, 1},
        .frames = {
            {.slice_index = {4, 0}, .seconds = 0.16f, .anchors = {{"pivot", {0.5f, 0.5f}}}},
            {.slice_index = {5, 0}, .seconds = 0.16f, .anchors = {{"pivot", {0.5f, 0.5f}}}},
            {.slice_index = {6, 0}, .seconds = 0.16f, .anchors = {{"pivot", {0.5f, 0.5f}}}},
            {.slice_index = {7, 0}, .seconds = 0.16f, .anchors = {{"pivot", {0.5f, 0.5f}}}},
            {.slice_index = {8, 0}, .seconds = 0.16f, .anchors = {{"pivot", {0.5f, 0.5f}}}},
        },
    };

    // animators
    wizard_missle1_animator = CreateAnimator({
        idle_animation_definition,
        walk_animation_definition,
        die_animation_definition,
    });
}

/**
 * Search Replace to add new animations
 *
    const auto &attack1_texture = ([^_]+)_attack1_texture = CreateImage\(engine, "([^/]+)/([^/]+)/([^"]+)"\);
    const auto &attack1_texture = $1_attack1_texture = CreateImage(engine, "$2/$3/$4");
    const auto &die_texture = $1_die_texture = CreateImage(engine, "$2/$3/Dead.png");

    ([^_]+)_([^_]+)_texture = CreateImage\(engine, "([^/]+)/([^/]+)/([^"]+)"\);
    const auto &$2_texture = $1_$2_texture = CreateImage(engine, "$3/$4/$5");

 */