#include "systems/GameInput.hpp"

using namespace systems;

void GameInput::PreUpdate(const olc::hw::Mouse &mouse, const olc::hw::Keyboard &keyboard, float fElapsedTime)
{
    const auto screen_center = olc::vf2d{128.0f, 128.0f};

    if (keyboard.GetKey(olc::Key::ALT).bHeld && keyboard.GetKey(olc::Key::ENTER).bPressed)
    {
        requestToggleFullscreen = true;
    }

    if (keyboard.GetKey(olc::Key::SHIFT).bHeld)
    {
        // boost = true;
    }

    if (keyboard.GetKey(olc::Key::SPACE).bPressed)
    {
        switchCharacter = true;
    }

    if (keyboard.GetKey(olc::Key::F1).bPressed)
    {
        cheatMode = !cheatMode;
    }
    if (keyboard.GetKey(olc::Key::Q).bPressed)
    {
        cheatSpawnEnemy = true;
        cheatSpawnEnemyType = enums::EnemyType::GHOST;
    }
    if (keyboard.GetKey(olc::Key::W).bPressed)
    {
        cheatSpawnEnemy = true;
        cheatSpawnEnemyType = enums::EnemyType::MINOTAUR;
    }
    if (keyboard.GetKey(olc::Key::E).bPressed)
    {
        cheatSpawnEnemy = true;
        cheatSpawnEnemyType = enums::EnemyType::SATYR;
    }
    if (keyboard.GetKey(olc::Key::R).bPressed)
    {
        cheatSpawnEnemy = true;
        cheatSpawnEnemyType = enums::EnemyType::SKELETON;
    }
    if (keyboard.GetKey(olc::Key::T).bPressed)
    {
        cheatSpawnEnemy = true;
        cheatSpawnEnemyType = enums::EnemyType::VAMPIRE;
    }
    if (keyboard.GetKey(olc::Key::Z).bPressed)
    {
        cheatSpawnEnemy = true;
        cheatSpawnEnemyType = enums::EnemyType::WEREWOLF;
    }

    const auto mouse_position = mouse.GetPosition();
    const auto diff = mouse_position - screen_center;
    const auto force = diff * fElapsedTime / 5.0f * diff.mag2() / 10000.0f;
    // acceleration = force;
}

void GameInput::PostUpdate(float fElapsedTime)
{
    requestToggleFullscreen = false;
    switchCharacter = false;
    cheatSpawnEnemy = false;
    // boost = false;
}