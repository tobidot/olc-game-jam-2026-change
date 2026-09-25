#pragma once
#include "./olcPixelGameEngine3.h"

struct AnimationFrame
{
    size_t index = 0;
    float seconds = 0.33f;
    std::unordered_map<std::string, olc::vf2d> anchors;
    olc::vf2d top_left;
    olc::vf2d top_right;
    olc::vf2d bottom_left;
    olc::vf2d bottom_right;
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
    float current_time = 0.f;
    std::string current_animation_index;
    std::unordered_map<std::string, Animation> animations;

public:
    virtual ~Animator() = default;

public:
    static std::unordered_map<std::string, Animation> MakeDefaultAnimations();

public:
    void Update(float elapsed_time);
    void SetAnimation(const std::string &name);
    Animation GetAnimation() const;
    AnimationFrame GetAnimationFrame() const;
    olc::ImageRegion GetImage() const;
    olc::vf2d GetImageAnchor(const std::string &anchor) const;
    bool HasImageAnchor(const std::string &anchor) const;
    olc::vf2d GetImagePivot() const;
    std::vector<std::string> GetAnimationNames() const;

public:
    Animation CreateAnimation(const std::string &name, std::shared_ptr<olc::Image> image,
                              const std::vector<AnimationFrame> &frames);
};