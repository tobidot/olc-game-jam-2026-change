// Define OLC_PGE3_APPLICATION to include the implementation of
// the Pixel Game Engine as part of this translation unit
#define OLC_PGE3_APPLICATION
#include "core/AssetManager.hpp"
#include "core/Geometry.hpp"
#include "exceptions/Exceptions.hpp"
#include "olc/olcPixelGameEngine3.h"
#include "renderer/Renderer.hpp"
#include "state/App.hpp"
#include "systems/Camera.hpp"
#include "systems/GameInput.hpp"
#include "systems/GameWorld.hpp"
#include "systems/PhysicsWorld.hpp"

#include <numbers>
#include <random>

class Main : public olc::PixelGameEngine
{
private:
    std::unique_ptr<core::AssetManager> asset_manager;
    std::unique_ptr<state::App> app_state;
    std::unique_ptr<systems::GameInput> game_input;
    std::unique_ptr<systems::GameWorld> game_world;
    std::unique_ptr<systems::PhysicsWorld> physics_world;
    std::unique_ptr<systems::Camera> camera;
    std::unique_ptr<renderer::Renderer> renderer;

public:
    Main()
    {
        sAppName = "Main - Testing build";
        asset_manager = std::make_unique<core::AssetManager>();
        app_state = std::make_unique<state::App>();
        game_input = std::make_unique<systems::GameInput>();
        game_world = std::make_unique<systems::GameWorld>();
        physics_world = std::make_unique<systems::PhysicsWorld>();
        camera = std::make_unique<systems::Camera>();
        renderer = std::make_unique<renderer::Renderer>();
    }

protected:
public:
    // Called once at the start, so create things here
    bool OnUserCreate() override
    {
        std::cout << "Create Main Start\n";

        app_state->settings.screen_size = core::Vector(static_cast<olc::vf2d>(ScreenSize()));
        asset_manager->Load(*this);
        game_world->Load(*asset_manager, *app_state);

        std::cout << "Create Main Finished\n";
        return true;
    }

    // Called every frame, so update things here
    bool OnUserUpdate(float elapsed_time) override
    {
        try
        {
            elapsed_time = std::min(1.0f, std::max(0.0f, elapsed_time));

            game_input->PreUpdate(mouse, keyboard, elapsed_time);
            renderer->Draw(draw, *asset_manager, *app_state);
            game_world->Update(*asset_manager, *game_input, *app_state, elapsed_time);
            game_input->PostUpdate(elapsed_time);

            draw.Circle(mouse.GetPosition(), 10, olc::Colour::BLUE);

            return true;
        }
        catch (const exceptions::runtime::RuntimeException &exception)
        {
            std::cerr << "Exception: " << exception.what() << '\n';
            std::cerr << "Thrown from: " << exception.where().function_name() << '\n';
            std::cerr << "File: " << exception.where().file_name() << ":" << exception.where().line() << '\n';
            throw exception;
        }
    }
};

// Main entry point for the application
int main()
{
    // Construct demo application
    Main main;
    olc::PGEConfig config;
    // config.vScreenSize = {600, 450};
    // config.vPixelSize = {2, 2};
    config.vScreenSize = {300, 200};
    config.vPixelSize = {4, 4};
    // config.bFullScreenable = false;
    config.bResizeable = false;
    // Create "screen" of 256x240 "pixels"
    // with a pixel size of 4x4 actual screen pixels
    if (main.Construct(config))
    {
        // Start the application
        main.Start();
    }

    return 0;
}