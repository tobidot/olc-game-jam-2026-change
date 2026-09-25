// Define OLC_PGE3_APPLICATION to include the implementation of
// the Pixel Game Engine as part of this translation unit
#define OLC_PGE3_APPLICATION
#include "AssetManager.hpp"
#include "GameInput.hpp"
#include "Ship.hpp"
#include "World.hpp"
#include "olcPixelGameEngine3.h"

#include <numbers>
#include <random>

class Main : public olc::PixelGameEngine
{
private:
    std::unique_ptr<AssetManager> asset_manager;
    std::unique_ptr<GameInput> game_input;
    std::unique_ptr<World> world;
    olc::vf2d camera_position;

public:
    Main()
    {
        sAppName = "Main - Testing build";
        asset_manager = std::make_unique<AssetManager>();
        game_input = std::make_unique<GameInput>();
        world = std::make_unique<World>();
    }

protected:
public:
    // Called once at the start, so create things here
    bool OnUserCreate() override
    {
        asset_manager->Load(*this);
        world->Seed(0, *asset_manager);
        camera_position = world->player_ship->m_position;
        return true;
    }

    // Called every frame, so update things here
    bool OnUserUpdate(float f_elapsed_time) override
    {
        f_elapsed_time = std::min(1.0f, std::max(0.0f, f_elapsed_time));
        //
        game_input->PreUpdate(mouse, keyboard, f_elapsed_time);

        ApplyCamera();
        DrawBackground();

        // Update and draw the world
        world->Update(*game_input, f_elapsed_time);
        world->Draw(draw);

        UpdateCamera(f_elapsed_time);

        game_input->PostUpdate(f_elapsed_time);
        // Successful frame
        return true;
    }

    void ApplyCamera()
    {
        const auto window_size = config.vScreenSize;

        //
        olc::tf2d transform;
        transform.translate(-camera_position + window_size * 0.5f);
        draw.SetWorldTransform(transform);
    }

    void DrawBackground()
    {
        const auto window_size = config.vScreenSize;

        // Draw normal image at x2 size
        const auto bg_zoom = 2.0f;

        const auto image_region = olc::ImageRegion(*asset_manager->background_texture);
        const auto size = image_region.regionsize * (256.0f / (image_region.regionsize.y) * bg_zoom);
        const auto offset = window_size * 0.5f - size / bg_zoom;

        // Clear screen to a background color
        olc::Pixel background_color{12, 44, 111};
        draw.Clear(background_color);
        draw.ImageRect(image_region, {0.f, 0.f}, {512, 256});
    }

    void UpdateCamera(float f_elapsed_time)
    {
        camera_position = camera_position.lerp(world->player_ship->m_position, 10 * f_elapsed_time);
    }
};

// Main entry point for the application
int main()
{
    // Construct demo application
    Main main;
    olc::PGEConfig config;
    config.vScreenSize = {256, 256};
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