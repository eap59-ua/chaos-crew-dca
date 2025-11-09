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
    auto players = registry.view<Player, Position, Solid, Sprite>();
    
    for (auto playerEntity : players) {
    	auto &pos = players.get<Position>(playerEntity);
    	auto &solid = players.get<Solid>(playerEntity);
    	auto &sprite = players.get<Sprite>(playerEntity);
    	
    	sprite.rect.x = pos.x;
        sprite.rect.y = pos.y;
        DrawRectangleRec(sprite.rect, solid.color);
        DrawRectangleLinesEx(sprite.rect, 2, BLACK);
	}
}

void renderPlatforms(entt::registry& registry) {
    auto platforms = registry.view<Platform, Position, Solid, Sprite>();
    
    for (auto platformEntity : platforms) {
    	auto &pos = platforms.get<Position>(platformEntity);
    	auto &solid = platforms.get<Solid>(platformEntity);
    	auto &sprite = platforms.get<Sprite>(platformEntity);
    	
    	sprite.rect.x = pos.x;
        sprite.rect.y = pos.y;
        DrawRectangleRec(sprite.rect, solid.color);
        DrawRectangleLinesEx(sprite.rect, 2, BLACK);
	}
}

void renderDoors(entt::registry& registry) {
    auto door = registry.view<Door, Position, Solid, Sprite>();
    
    for (auto doorEntity : door) {
    	auto &pos = door.get<Position>(doorEntity);
    	auto &solid = door.get<Solid>(doorEntity);
    	auto &sprite = door.get<Sprite>(doorEntity);
    	
    	sprite.rect.x = pos.x;
        sprite.rect.y = pos.y;
        DrawRectangleRec(sprite.rect, solid.color);
        DrawRectangleLinesEx(sprite.rect, 2, BLACK);
        const char* text = "EXIT";
        int textWidth = MeasureText(text, 30);
        DrawText(text,
                sprite.rect.x + (sprite.rect.width - textWidth) / 2,
                sprite.rect.y + (sprite.rect.height - 30) / 2,
                30,
                WHITE);
	}
}

void renderScene(entt::registry& registry) {
    renderPlatforms(registry);
    renderPlayers(registry);
    renderDoors(registry);
}
