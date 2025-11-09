#include <raylib.h>
#include "../entt/entt.hpp"
#include "../components/Player.hpp"
#include "../components/Platform.hpp"
#include "../components/Position.hpp"
#include "../components/Velocity.hpp"
#include "../components/Solid.hpp"
#include "../components/Sprite.hpp"

void CollisionSystem(entt::registry& registry) {
    auto players = registry.view<Player, Position, Velocity, Solid>();
    auto platforms = registry.view<Platform, Position, Solid>();

    players.each([&](auto entity, Player& player, Position& pos, Velocity& vel, Solid& solid) {
        // Reset ground state
        player.onGround = false;

        // Guardar posición anterior para mejor detección
        float prevY = pos.y;
        float prevX = pos.x;

        platforms.each([&](const auto&, const Position& platPos, const Solid& platSolid) {
            Rectangle playerRect = {pos.x, pos.y, solid.width, solid.height};
            Rectangle platformRect = {platPos.x, platPos.y, platSolid.width, platSolid.height};
            
            if (CheckCollisionRecs(playerRect, platformRect)) {
                // Calcular solapamientos
                float overlapTop = (platPos.y + platSolid.height) - pos.y;
                float overlapBottom = (pos.y + solid.height) - platPos.y;
                float overlapLeft = (platPos.x + platSolid.width) - pos.x;
                float overlapRight = (pos.x + solid.width) - platPos.x;

                // Encontrar la menor penetración
                float minOverlap = fminf(fminf(overlapTop, overlapBottom), 
                                       fminf(overlapLeft, overlapRight));

                // Resolver según la menor penetración
                if (minOverlap == overlapBottom && vel.vy >= 0) {
                    // Colisión desde arriba
                    pos.y = platPos.y - solid.height;
                    vel.vy = 0;
                    player.onGround = true;
                }
                else if (minOverlap == overlapTop && vel.vy <= 0) {
                    // Colisión desde abajo
                    pos.y = platPos.y + platSolid.height;
                    vel.vy = 0;
                }
                else if (minOverlap == overlapLeft && vel.vx <= 0) {
                    // Colisión desde la derecha
                    pos.x = platPos.x + platSolid.width;
                    vel.vx = 0;
                }
                else if (minOverlap == overlapRight && vel.vx >= 0) {
                    // Colisión desde la izquierda
                    pos.x = platPos.x - solid.width;
                    vel.vx = 0;
                }
            }
        });
    });
}
