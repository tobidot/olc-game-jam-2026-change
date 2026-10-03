#include "core/Animator.hpp"

#include "exceptions/Exceptions.hpp"
#include "olc/olcPixelGameEngine3.h"

using namespace core;

Animation Animator::GetAnimation(const std::string &name) const
{
    const auto iterator = animations.find(name);
    if (iterator == animations.end())
    {
        throw exceptions::runtime::AnimationIndexNotFoundException(name);
    }
    return iterator->second;
}

AnimationFrame Animator::GetAnimationFrame(const std::string &name, float seconds) const
{
    auto animation = GetAnimation(name);
    auto time = seconds;
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

olc::ImageRegion Animator::GetImage(const std::string &name, float seconds) const
{
    const auto &animation = GetAnimation(name);
    const auto &frame = GetAnimationFrame(name, seconds);

    auto image = animation.image;
    if (!image)
    {
        throw exceptions::runtime::AnimationIsMissingSourceImageException();
    }

    return olc::ImageRegion{
        *image,
        frame.top_left,
        frame.top_right,
        frame.bottom_left,
        frame.bottom_right,
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

bool Animator::HasImageAnchor(const std::string &name, float seconds, const std::string &anchor) const
{
    auto frame = GetAnimationFrame(name, seconds);
    return frame.anchors.contains(anchor);
}

core::Vector Animator::GetImageAnchor(const std::string &name, float seconds, const std::string &anchor) const
{
    auto frame = GetAnimationFrame(name, seconds);
    return frame.anchors.at(anchor);
}

core::Vector Animator::GetImagePivot(const std::string &name, float seconds) const
{
    auto frame = GetAnimationFrame(name, seconds);
    if (HasImageAnchor(name, seconds, "pivot"))
    {
        return GetImageAnchor(name, seconds, "pivot");
    }
    auto animation = GetAnimation(name);
    return core::Vector((frame.bottom_right - frame.top_left) * 0.5f * animation.image->Size());
}

Animation Animator::CreateAnimation(
    const std::string &name, std::shared_ptr<olc::Image> image, const std::vector<AnimationFrame> &frames
)
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

float Animator::GetAnimationSpeedForDuration(const std::string &name, float target_duration) const
{
    const auto &animation = GetAnimation(name);
    return animation.total_seconds / target_duration;
}

std::pair<float, float>
Animator::GetFrameWindowTime(const std::string &name, size_t frame_start, size_t frame_end) const
{
    const auto &animation = GetAnimation(name);
    if (frame_end < frame_start)
    {
        throw exceptions::runtime::AnimationInvalidFrameWindowException("Starting frame may not be after ending frame");
    }
    if (frame_start >= animation.frames.size())
    {
        throw exceptions::runtime::AnimationFrameIndexNotFoundException(name, frame_start);
    }
    if (frame_end >= animation.frames.size())
    {
        throw exceptions::runtime::AnimationFrameIndexNotFoundException(name, frame_end);
    }

    float start = 0.0f;
    float end = animation.total_seconds;
    float current_seconds = 0.0f;
    for (size_t i = 0; i <= frame_end; ++i)
    {
        const auto &frame = animation.frames.at(i);
        if (i == frame_start)
        {
            start = current_seconds;
        }
        current_seconds += frame.seconds;
        if (i == frame_end)
        {
            end = current_seconds;
        }
    }
    return std::make_pair(start, end);
}