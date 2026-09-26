#pragma once
#include "olcPixelGameEngine3.h"

class GameInput
{
public:
    olc::vf2d acceleration = {0, 0};
    bool boost = false;
    bool requestToggleFullscreen = false;

public:
    void PreUpdate(
        const olc::hw::Mouse &mouse,
        const olc::hw::Keyboard &keyboard,
        float fElapsedTime
    );
    void PostUpdate(float fElapsedTime);
};