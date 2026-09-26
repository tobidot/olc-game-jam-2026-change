#pragma once

#include "enums/Enums.hpp"

#include <vector>

namespace state
{

struct Account
{

    std::vector<enums::Level> unlocked_level;
};

} // namespace state
