#pragma once
#include "core/Geometry.hpp"

namespace state
{

struct Settings
{
    core::Vector screen_size = {256.f, 256.f};
    float music_volume = .35f;
    float sfx_volume = 0.8f;
};

} // namespace state