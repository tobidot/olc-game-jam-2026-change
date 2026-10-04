#include "renderer/HudRenderer.hpp"

#include "core/AssetManager.hpp"
#include "entities/Entity.hpp"
#include "helper.hpp"

using namespace renderer;

void HudRenderer::Draw(olc::Draw &draw, const core::AssetManager &assets, const state::App &state) const
{
    auto health_bar_color = olc::Pixel(0xff000088);
    const auto player = state.game_world.player_entity->ref;
    const auto health_height = player->health / player->max_health * 44.0f;
    const auto offset = olc::vf2d{5, 5};
    // live bar
    draw.FilledRoundedRect(offset + olc::vf2d{0, 0}, {20, 50}, 8, olc::Colour::BLACK);
    draw.FilledRoundedRect(offset + olc::vf2d{3, 47 - health_height}, {14, health_height}, 3, health_bar_color);

    DrawCharacterSlot(
        draw,
        assets,
        olc::vf2d{45, 20},
        state.game_world.player.character_slot_left,
        state.game_world.player.character_slot_left_cooldown
    );

    DrawCharacterSlot(
        draw,
        assets,
        olc::vf2d{80, 20},
        state.game_world.player.character_slot_right,
        state.game_world.player.character_slot_right_cooldown
    );
}

void HudRenderer::DrawCharacterSlot(
    olc::Draw &draw,
    const core::AssetManager &assets,
    const olc::vf2d &center,
    enums::CharacterType next,
    float cooldown
) const
{
    draw.FilledRect(center - olc::vf2d{15, 15}, {30, 30}, olc::Colour::BLACK);

    auto image = assets.knight_animator->GetImage("idle", 0.f);
    switch (next)
    {
        case enums::CharacterType::KNIGHT:
        {
            image = olc::ImageRegion(
                *assets.knight_idle_texture,
                olc::vf2d{0.04, 0.5},
                olc::vf2d{0.08, 0.5},
                olc::vf2d{0.04, 0.66},
                olc::vf2d{0.08, 0.66}
            );
            break;
        }
        case enums::CharacterType::SHINOBI:
        {
            image = olc::ImageRegion(
                *assets.shinobi_idle_texture,
                olc::vf2d{0.07, 0.45},
                olc::vf2d{0.1, 0.45},
                olc::vf2d{0.07, 0.63},
                olc::vf2d{0.1, 0.63}
            );
            break;
        }
        case enums::CharacterType::SAMURAI:
        {
            image = olc::ImageRegion(
                *assets.samurai_idle_texture,
                olc::vf2d{0.025, 0.45},
                olc::vf2d{0.055, 0.45},
                olc::vf2d{0.025, 0.63},
                olc::vf2d{0.055, 0.63}
            );
            break;
        }
        case enums::CharacterType::WIZARD:
        {
            image = olc::ImageRegion(
                *assets.wizard_idle_texture,
                olc::vf2d{0.05, 0.48},
                olc::vf2d{0.07, 0.48},
                olc::vf2d{0.05, 0.64},
                olc::vf2d{0.07, 0.64}
            );
            break;
        }
        default:
            throw std::runtime_error("Unknown Character type");
    }

    draw.ImageRect(image, center - olc::vf2d(13, 13), {26, 26});

    if (cooldown > 0.0f)
    {
        const auto cooldown_percent = cooldown / state::Player::CHARACTER_SLOT_MAX_COOLDOWN;
        const auto radians = cooldown_percent * 2 * Const::PI;
        FillArc(draw, center, 12, radians, olc::Pixel(0xBBBBBB44));
    }
}

void HudRenderer::FillArc(
    olc::Draw &draw, const olc::vf2d &center, float radius, float radians, const olc::Pixel &color
) const
{
    std::vector<olc::vf2d> quad_points;
    quad_points.reserve(32);

    quad_points.push_back(center);
    for (size_t i = 0; i < 30; ++i)
    {
        const auto radiant = 2 * Const::PI - (radians * static_cast<float>(i) / 30.f) + Const::PI * 1.5f;
        const auto point = center + olc::vf2d(cos(radiant) * radius, sin(radiant) * radius);
        quad_points.push_back(point);
    }
    quad_points.push_back(center);

    draw.FilledPolygon(olc::Structure::Fan, quad_points, color);
}