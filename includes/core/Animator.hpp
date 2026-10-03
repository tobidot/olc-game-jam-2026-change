#pragma once
#include "core/Geometry.hpp"
#include "olc/olcPixelGameEngine3.h"

#include <memory>

namespace core
{

struct AnimationFrameDefinition
{
    olc::vi2d slice_index = {0, 0};
    float seconds = 0.33f;
    std::unordered_map<std::string, core::Vector> anchors;
};

struct AnimationDefinition
{
    std::string name;
    std::shared_ptr<olc::Image> image;
    olc::vi2d frame_slices;
    std::vector<AnimationFrameDefinition> frames;
};

struct AnimationFrame
{
    size_t index = 0;
    float seconds = 0.33f;
    std::unordered_map<std::string, core::Vector> anchors;
    core::Vector top_left;
    core::Vector top_right;
    core::Vector bottom_left;
    core::Vector bottom_right;
};

struct Animation
{
    std::string name;
    float total_seconds = 0.0f;
    std::shared_ptr<olc::Image> image;
    std::vector<AnimationFrame> frames;
};

class Animator
{

public:
    std::unordered_map<std::string, Animation> animations;

public:
    [[nodiscard]]
    Animation GetAnimation(const std::string &name) const;
    [[nodiscard]]
    AnimationFrame GetAnimationFrame(const std::string &name, float seconds) const;
    [[nodiscard]]
    olc::ImageRegion GetImage(const std::string &name, float seconds) const;
    [[nodiscard]]
    core::Vector GetImageAnchor(const std::string &name, float seconds, const std::string &anchor) const;
    [[nodiscard]]
    bool HasImageAnchor(const std::string &name, float seconds, const std::string &anchor) const;
    [[nodiscard]]
    core::Vector GetImagePivot(const std::string &name, float seconds) const;
    [[nodiscard]]
    std::vector<std::string> GetAnimationNames() const;
    [[nodiscard]]
    std::pair<float, float> GetFrameWindowTime(const std::string &name, size_t frame_start, size_t frame_end) const;
    [[nodiscard]]
    float GetAnimationSpeedForDuration(const std::string &name, float target_duration) const;

public:
    Animation CreateAnimation(
        const std::string &name, std::shared_ptr<olc::Image> image, const std::vector<AnimationFrame> &frames
    );
};

} // namespace core
