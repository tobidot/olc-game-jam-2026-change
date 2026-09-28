#pragma once
#include "olc/olcPixelGameEngine3.h"

namespace systems
{

class GameInput
{
public:
    bool requestToggleFullscreen = false;
    bool switchCharacter = false;

public:
    void PreUpdate(const olc::hw::Mouse &mouse, const olc::hw::Keyboard &keyboard, float fElapsedTime);
    void PostUpdate(float fElapsedTime);
};

} // namespace systems