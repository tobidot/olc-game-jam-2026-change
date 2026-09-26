#pragma once
#include <exception>
#include <sstream>
#include <string>

namespace exceptions
{

namespace runtime
{
using ss = std::stringstream;

class AnimationIndexNotFoundException : public std::runtime_error
{
public:
    explicit AnimationIndexNotFoundException(const std::string &index)
        : std::runtime_error((ss() << "Animation name '" << index << "' not found").str())
    {
    }
};

class AnimationIsMissingSourceImageException : public std::runtime_error
{
public:
    AnimationIsMissingSourceImageException() : std::runtime_error("Animation is missing a source image")
    {
    }
};

class AnimationHasNoFramesException : public std::runtime_error
{
public:
    AnimationHasNoFramesException() : std::runtime_error("Animation has no frames defined")
    {
    }
};

} // namespace runtime

namespace logic
{

} // namespace logic

} // namespace exceptions
