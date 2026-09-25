#include "Animator.hpp"

#include "Exceptions.hpp"
#include "olcPixelGameEngine3.h"

std::unordered_map<std::string, Animation> Animator::MakeDefaultAnimations()
{
    const std::string default_animation_name = std::string("none");
    auto animation_frame = AnimationFrame{
        .seconds = 1.0f,
        .top_left = {1.0f, 1.0f},
        .top_right = {1.0f, 1.0f},
        .bottom_left = {1.0f, 1.0f},
        .bottom_right = {1.0f, 1.0f},
    };
    auto animation = Animation{
        .name = default_animation_name,
        .total_seconds = 1.0f,
        .frames = std::vector<AnimationFrame>({
            animation_frame,
        }),
    };
    auto animation_pair = std::make_pair(default_animation_name, animation);
    return std::unordered_map<std::string, Animation>({animation_pair});
}

void Animator::Update(float elapsed_time)
{
    const auto animation = GetAnimation();
    const auto animation_time = animation.total_seconds;
    if (animation_time <= 0.0f)
    {
        current_time = 0.0f;
        return;
    }
    current_time = static_cast<float>(fmod(current_time + elapsed_time, animation_time));
}

void Animator::SetAnimation(const std::string &name)
{
    if (!animations.contains(name))
    {
        throw exceptions::runtime::AnimationIndexNotFoundException(name);
    }
    current_animation_index = name;
}

Animation Animator::GetAnimation() const
{
    return animations.at(current_animation_index);
}

AnimationFrame Animator::GetAnimationFrame() const
{
    auto animation = GetAnimation();
    auto time = current_time;
    if (animation.frames.empty())
    {
        throw exceptions::runtime::AnimationHasNoFramesException();
    }
    for (const auto &frame : animation.frames)
    {
        time -= frame.seconds;
        if (time <= 0.0f)
        {
            return frame;
        }
    }
    return *animation.frames.begin();
}

olc::ImageRegion Animator::GetImage() const
{
    const auto &animation = GetAnimation();
    const auto &frame = GetAnimationFrame();

    auto image = animation.image;
    if (!image)
    {
        throw exceptions::runtime::AnimationIsMissingSourceImageException();
    }

    return olc::ImageRegion{
        *image, frame.top_left, frame.top_right, frame.bottom_left, frame.bottom_right,
    };
}

std::vector<std::string> Animator::GetAnimationNames() const
{
    std::vector<std::string> keys;
    keys.reserve(animations.size());

    for (const auto &key_value : animations)
    {
        keys.push_back(key_value.first);
    }
    return keys;
}

bool Animator::HasImageAnchor(const std::string &anchor) const
{
    auto frame = GetAnimationFrame();
    return frame.anchors.contains(anchor);
}

olc::vf2d Animator::GetImageAnchor(const std::string &anchor) const
{
    auto frame = GetAnimationFrame();
    return frame.anchors.at(anchor);
}

olc::vf2d Animator::GetImagePivot() const
{
    auto frame = GetAnimationFrame();
    if (HasImageAnchor("pivot"))
    {
        return GetImageAnchor("pivot");
    }
    auto animation = GetAnimation();
    return (frame.bottom_right - frame.top_left) * 0.5f * animation.image->Size();
}

Animation Animator::CreateAnimation(const std::string &name, std::shared_ptr<olc::Image> image,
                                    const std::vector<AnimationFrame> &frames)
{
    Animation animation;
    animation.image = std::move(image);
    animation.name = name;
    size_t index = 0;
    float total_seconds = 0.0f;
    for (const auto &frame_definition : frames)
    {
        AnimationFrame frame_value{
            .index = index++,
            .seconds = frame_definition.seconds,
            .anchors = frame_definition.anchors,
            .top_left = frame_definition.top_left,
            .top_right = frame_definition.top_right,
            .bottom_left = frame_definition.bottom_left,
            .bottom_right = frame_definition.bottom_right,
        };
        animation.frames.push_back(frame_value);
        total_seconds += frame_definition.seconds;
    }
    animation.total_seconds = total_seconds;
    animations.emplace(std::pair<std::string, Animation>(name, animation));

    return animation;
}