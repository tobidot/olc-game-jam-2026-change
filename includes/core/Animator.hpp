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
    Animation GetAnimation(const std::string &name) const;
    AnimationFrame GetAnimationFrame(const std::string &name, float seconds) const;
    olc::ImageRegion GetImage(const std::string &name, float seconds) const;
    olc::vf2d GetImageAnchor(const std::string &name, float seconds, const std::string &anchor) const;
    bool HasImageAnchor(const std::string &name, float seconds, const std::string &anchor) const;
    olc::vf2d GetImagePivot(const std::string &name, float seconds) const;
    std::vector<std::string> GetAnimationNames() const;

public:
    Animation CreateAnimation(
        const std::string &name, std::shared_ptr<olc::Image> image, const std::vector<AnimationFrame> &frames
    );
};

} // namespace core
