#pragma once
#include <exception>
#include <sstream>
#include <string>

namespace exceptions
{

namespace runtime
{
using ss = std::stringstream;

class RuntimeException : public std::runtime_error
{
public:
    std::source_location location;

public:
    explicit RuntimeException(
        const std::string &message, std::source_location location = std::source_location::current()
    )
        : std::runtime_error(message), location(location) {

          };

    [[nodiscard]]
    const std::source_location &where() const noexcept
    {
        return location;
    }
};

class AnimationIndexNotFoundException : public RuntimeException
{
public:
    explicit AnimationIndexNotFoundException(
        const std::string &index, std::source_location location = std::source_location::current()
    )
        : RuntimeException((ss() << "Animation name '" << index << "' not found").str(), location)
    {
    }
};

class AnimationIsMissingSourceImageException : public RuntimeException
{
public:
    explicit AnimationIsMissingSourceImageException(std::source_location location = std::source_location::current())
        : RuntimeException("Animation is missing a source image", location)
    {
    }
};

class AnimationHasNoFramesException : public RuntimeException
{
public:
    explicit AnimationHasNoFramesException(std::source_location location = std::source_location::current())
        : RuntimeException("Animation has no frames defined", location)
    {
    }
};

class AnimationFrameIndexNotFoundException : public RuntimeException
{
public:
    explicit AnimationFrameIndexNotFoundException(
        const std::string &animation, size_t index, std::source_location location = std::source_location::current()
    )
        : RuntimeException(
              (ss() << "Frame '" << index << "' not found in animation '" << animation << "'").str(), location
          )
    {
    }
};

class AnimationInvalidFrameWindowException : public RuntimeException
{
public:
    explicit AnimationInvalidFrameWindowException(
        const std::string &message, std::source_location location = std::source_location::current()
    )
        : RuntimeException(message, location)
    {
    }
};

} // namespace runtime

namespace logic
{

class LogicException : public std::logic_error
{
public:
    std::source_location location;

public:
    explicit LogicException(const std::string &message, std::source_location location = std::source_location::current())
        : std::logic_error(message), location(location) {

          };

    [[nodiscard]]
    const std::source_location &where() const noexcept
    {
        return location;
    }
};

} // namespace logic

} // namespace exceptions
