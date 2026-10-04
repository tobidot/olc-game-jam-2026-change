#include "renderer/GameWorldRenderer.hpp"

#include "core/AssetManager.hpp"
#include "helper.hpp"

using namespace renderer;

void GameWorldRenderer::Draw(olc::Draw &draw, const core::AssetManager &assets, const state::App &state) const
{
    DrawBackground(draw, assets, state);

    // move the camera with the scroll position
    olc::tf2d world_transform = draw.GetWorldTransform();
    world_transform.push(olc::mf3d::translation(core::Vector{-state.game_world.player.level_progress, 0.f}));
    draw.SetWorldTransform(world_transform);

    auto sorted_entites = std::vector(state.game_world.entities);
    std::ranges::sort(
        sorted_entites,
        [](const std::shared_ptr<entity::EntityHandle> &first, const std::shared_ptr<entity::EntityHandle> &second)
        {
            // draw effects on top
            if (first->ref->target_type == enums::TargetType::EFFECT &&
                second->ref->target_type != enums::TargetType::EFFECT)
            {
                return false;
            }
            if (first->ref->target_type != enums::TargetType::EFFECT &&
                second->ref->target_type == enums::TargetType::EFFECT)
            {
                return true;
            }
            return first->ref->position.y < second->ref->position.y;
        }
    );

    for (const auto &entity : sorted_entites)
    {
        DrawEntity(draw, assets, state, *entity->ref);
    }

    world_transform.pop();
    draw.SetWorldTransform(world_transform);
}

void GameWorldRenderer::DrawEntity(
    olc::Draw &draw, const core::AssetManager &assets, const state::App &state, const entity::Entity &entity
) const
{
    // transform view to entity
    olc::tf2d world_transform = draw.GetWorldTransform();
    const auto top_offset = state.game_world.render_offset_top;
    const auto screen_height = state.settings.screen_size.y - top_offset;
    const auto position_scale = core::Vector{1.0f, screen_height / state.game_world.boundaries.Height()};
    const auto position = entity.position * position_scale + core::Vector{0.f, top_offset};
    world_transform.push(olc::mf3d::translation(position));
    draw.SetWorldTransform(world_transform);

    // build a tint
    auto tint = olc::Colour::WHITE;
    if (entity.damage_animation_time < entity.damage_animation_duration)
    {
        auto red = olc::Colour::RED;
        auto t = cosf(entity.damage_animation_time * Const::PI * 16.0f);
        tint = (red * t + tint * (1 - t));
    }
    if (entity.heal_animation_time < entity.heal_animation_duration)
    {
        auto green = olc::Colour::GREEN;
        auto t = cosf(entity.heal_animation_time * Const::PI * 4.0f);
        tint = (green * t + tint * (1 - t));
    }

    // draw the image
    const auto &image = entity.GetImage();
    const auto flipped = entity.is_flipped ? core::Vector{-1.f, 1.f} : core::Vector{1.f, 1.f};
    const auto pivot = entity.animator.GetImagePivot(entity.current_animation, entity.current_animation_time);
    const auto pivot_pixels = -pivot * image.regionsize * entity.scale * flipped;
    draw.Image(image, pivot_pixels + core::Vector{0.f, -entity.z_offset}, entity.scale * flipped, tint);
    draw.FilledEllipse({0, 0}, entity.shape.size.x, entity.shape.size.y * 0.5f, olc::Pixel(0x22000088));

    world_transform.pop();
    draw.SetWorldTransform(world_transform);
}

void GameWorldRenderer::DrawBackground(olc::Draw &draw, const core::AssetManager &assets, const state::App &state) const
{
    // get the window sizes
    const auto window_size = core::Vector(state.settings.screen_size);
    const auto window_top_left = core::Vector(0.f, 0.f);
    const auto window_bottom_right = window_size;

    // get the background image sizes
    const auto background_image = assets.background_jungle_texture;
    const auto background_image_size = core::Vector(background_image->Size());

    // what is the scaling to fit the background onto the screen
    const auto background_scale_factor = window_size.y / background_image_size.y;
    const auto background_scaled_image_width = background_image_size.x * background_scale_factor;

    // which part of the background should be shown (0 - 1)
    const auto scroll_offset_start = state.game_world.player.level_progress / background_scaled_image_width;
    const auto scroll_offset_end = scroll_offset_start + (window_size.x / background_scaled_image_width);

    // define the source rect
    const auto source_tl = core::Vector(scroll_offset_start, 0.f);
    const auto source_tr = core::Vector(scroll_offset_end, 0.f);
    const auto source_bl = core::Vector(scroll_offset_start, 1.f);
    const auto source_br = core::Vector(scroll_offset_end, 1.f);
    const auto image_region = olc::ImageRegion(*background_image, source_tl, source_tr, source_bl, source_br);

    draw.ImageRect(image_region, olc::vi2d(window_top_left), olc::vi2d(window_size));
}