#pragma once
#include "core/Animator.hpp"
#include "olc/olcPixelGameEngine3.h"

#include <memory>

namespace core
{

class AssetManager
{
public:
    // plain images
    std::shared_ptr<olc::Image> background_jungle_texture;
    std::shared_ptr<olc::Image> hero_knight_idle_texture;
    std::shared_ptr<olc::Image> hero_knight_walk_texture;
    std::shared_ptr<olc::Image> enemy_ghost_idle_texture;
    std::shared_ptr<olc::Image> enemy_ghost_walk_texture;
    std::shared_ptr<olc::Image> enemy_ghost_attack1_texture;
    // animations
    std::shared_ptr<Animator> hero_knight_animator;
    std::shared_ptr<Animator> enemy_ghost_animator;

public:
    void Load(olc::PixelGameEngine &engine);
    void LoadKnight(olc::PixelGameEngine &engine);
    void LoadGhost(olc::PixelGameEngine &engine);
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