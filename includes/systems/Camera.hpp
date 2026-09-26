#pragma once

#include "state/App.hpp"

namespace systems
{

class Camera
{
    void Upadte(state::App &app_state, float elapsed_time);
};

} // namespace systems