#include <raylib.h>
#include "../entt/entt.hpp"
#include "../components/Player.hpp"
#include "../components/Platform.hpp"
#include "../components/Door.hpp"
#include "../components/Position.hpp"
#include "../components/Sprite.hpp"
#include "../components/Velocity.hpp"
#include "../components/Solid.hpp"
#include "RenderSystem.hpp"

void renderPlayers(entt::registry& registry) {
    auto view = registry.view<Player, Position, Solid, Sprite>();
    
    view.each([&registry](const auto entity, const Position& pos, const Solid& solid, Sprite& sprite) {
        // Draw player rectangle
        sprite.rect.x = pos.x;
        sprite.rect.y = pos.y;
        DrawRectangleRec(sprite.rect, solid.color);
        DrawRectangleLinesEx(sprite.rect, 2, BLACK);
    });
}

void renderPlatforms(entt::registry& registry) {
    auto view = registry.view<Platform, Position, Solid, Sprite>();
    
    view.each([](const auto&, const Position& pos, const Solid& solid, const Sprite& sprite) {
        // Draw platform rectangle
        DrawRectangleRec(sprite.rect, solid.color);
        DrawRectangleLinesEx(sprite.rect, 2, BLACK);
    });
}

void renderDoors(entt::registry& registry) {
    auto view = registry.view<Door, Position, Solid, Sprite>();
    
    view.each([](const auto&, const Position& pos, const Solid& solid, const Sprite& sprite) {
        // Draw door rectangle
        DrawRectangleRec(sprite.rect, solid.color);
        DrawRectangleLinesEx(sprite.rect, 2, DARKGREEN);
        const char* text = "EXIT";
        int textWidth = MeasureText(text, 30);
        DrawText(text,
                sprite.rect.x + (sprite.rect.width - textWidth) / 2,
                sprite.rect.y + (sprite.rect.height - 30) / 2,
                30,
                WHITE);
    });
}

void renderScene(entt::registry& registry) {
    renderPlatforms(registry);
    renderPlayers(registry);
    renderDoors(registry);
}