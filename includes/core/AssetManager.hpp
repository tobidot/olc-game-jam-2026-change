#pragma once
#include "core/Animator.hpp"
#include "olc/miniaudio.h"
#include "olc/olcPGEX3_Miniaudio.h"
#include "olc/olcPixelGameEngine3.h"

#include <memory>

namespace core
{

class AssetManager
{
public:
    // sounds
    std::shared_ptr<olc::ext::Miniaudio::Sound> sfx_select;
    // plain images
    std::shared_ptr<olc::Image> background_jungle_texture;
    //
    std::shared_ptr<olc::Image> knight_idle_texture;
    std::shared_ptr<olc::Image> knight_walk_texture;
    std::shared_ptr<olc::Image> knight_attack1_texture;
    std::shared_ptr<olc::Image> samurai_idle_texture;
    std::shared_ptr<olc::Image> samurai_walk_texture;
    std::shared_ptr<olc::Image> samurai_attack1_texture;
    std::shared_ptr<olc::Image> shinobi_idle_texture;
    std::shared_ptr<olc::Image> shinobi_walk_texture;
    std::shared_ptr<olc::Image> shinobi_attack1_texture;
    std::shared_ptr<olc::Image> wizard_idle_texture;
    std::shared_ptr<olc::Image> wizard_walk_texture;
    std::shared_ptr<olc::Image> wizard_attack1_texture;
    //
    std::shared_ptr<olc::Image> ghost_idle_texture;
    std::shared_ptr<olc::Image> ghost_walk_texture;
    std::shared_ptr<olc::Image> ghost_attack1_texture;
    std::shared_ptr<olc::Image> minotaur_idle_texture;
    std::shared_ptr<olc::Image> minotaur_walk_texture;
    std::shared_ptr<olc::Image> minotaur_attack1_texture;
    std::shared_ptr<olc::Image> skeleton_idle_texture;
    std::shared_ptr<olc::Image> skeleton_walk_texture;
    std::shared_ptr<olc::Image> skeleton_attack1_texture;
    std::shared_ptr<olc::Image> satyr_idle_texture;
    std::shared_ptr<olc::Image> satyr_walk_texture;
    std::shared_ptr<olc::Image> satyr_attack1_texture;
    std::shared_ptr<olc::Image> werewolf_idle_texture;
    std::shared_ptr<olc::Image> werewolf_walk_texture;
    std::shared_ptr<olc::Image> werewolf_attack1_texture;
    std::shared_ptr<olc::Image> vampire_idle_texture;
    std::shared_ptr<olc::Image> vampire_walk_texture;
    std::shared_ptr<olc::Image> vampire_attack1_texture;
    // animations
    std::shared_ptr<Animator> knight_animator;
    std::shared_ptr<Animator> samurai_animator;
    std::shared_ptr<Animator> shinobi_animator;
    std::shared_ptr<Animator> wizard_animator;
    std::shared_ptr<Animator> ghost_animator;
    std::shared_ptr<Animator> minotaur_animator;
    std::shared_ptr<Animator> skeleton_animator;
    std::shared_ptr<Animator> satyr_animator;
    std::shared_ptr<Animator> werewolf_animator;
    std::shared_ptr<Animator> vampire_animator;

public:
    void Load(olc::PixelGameEngine &engine, olc::ext::Miniaudio::AudioEngine &audio);
    void LoadKnight(olc::PixelGameEngine &engine);
    void LoadShinobi(olc::PixelGameEngine &engine);
    void LoadSamurai(olc::PixelGameEngine &engine);
    void LoadWizard(olc::PixelGameEngine &engine);
    void LoadVampire(olc::PixelGameEngine &engine);
    void LoadGhost(olc::PixelGameEngine &engine);
    void LoadMinotaur(olc::PixelGameEngine &engine);
    void LoadSkeleton(olc::PixelGameEngine &engine);
    void LoadSatyr(olc::PixelGameEngine &engine);
    void LoadWerewolf(olc::PixelGameEngine &engine);
    void LoadSelectSound(olc::ext::Miniaudio::AudioEngine &audio);
    [[nodiscard]]
    std::shared_ptr<olc::Image> CreateImage(olc::PixelGameEngine &engine, const char *path) const;
    [[nodiscard]]
    std::shared_ptr<Animator> CreateAnimator(const std::vector<AnimationDefinition> &animations) const;
    [[nodiscard]]
    AnimationDefinition CreateSimpleAnimation(
        const std::shared_ptr<olc::Image> &texture,
        const std::string &name,
        const olc::vi2d &slices,
        const std::unordered_map<std::string, core::Vector> &anchors,
        const std::vector<float> &delays_per_frame,
        int frame_count = 0
    ) const;

private:
    [[nodiscard]] std::string_view Trim(const std::string_view &input, char remove) const;

    /**
     * Build the asset path from multiple parts
     */
    template <typename... T>
        requires(std::is_convertible_v<T, std::string_view> && ...)
    [[nodiscard]]
    std::string MakeAssetPath(T... parts) const
    {
        std::stringstream buffer;
        buffer << "assets";

        AppendParts(buffer, parts...);

        return buffer.str();
    };

    template <typename... T>
        requires(std::is_convertible_v<T, std::string_view> && ...)
    void AppendParts(std::stringstream &stream, const std::string_view &next, T... parts) const
    {
        stream << '/' << Trim(next, '/');
    };

    /**
     * End Condition if no more parts exist
     */
    void AppendParts(std::stringstream &stream) const {};
};

} // namespace core