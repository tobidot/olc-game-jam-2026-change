#include "GameInput.hpp"

void GameInput::PreUpdate(
    const olc::hw::Mouse &mouse,
    const olc::hw::Keyboard &keyboard,
    float fElapsedTime)
{
    const auto screen_center = olc::vf2d{128.0f, 128.0f};

    if (keyboard.GetKey(olc::Key::ALT).bHeld && keyboard.GetKey(olc::Key::ENTER).bPressed)
    {
        requestToggleFullscreen = true;
    }

    if (keyboard.GetKey(olc::Key::SHIFT).bHeld)
    {
        boost = true;
    }

    const auto mouse_position = mouse.GetPosition();
    const auto diff = mouse_position - screen_center;
    const auto force = diff * fElapsedTime / 5.0f * diff.mag2() / 10000.0f;
    acceleration = force;
}

void GameInput::PostUpdate(float fElapsedTime)
{
    requestToggleFullscreen = false;
    boost = false;
}