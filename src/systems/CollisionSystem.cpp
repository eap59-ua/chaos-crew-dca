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

    for(auto playerEntity : players) {
        auto& player = players.get<Player>(playerEntity);
        auto& pos = players.get<Position>(playerEntity);
        auto& vel = players.get<Velocity>(playerEntity);
        auto& solid = players.get<Solid>(playerEntity);

        // Reset ground state
        player.onGround = false;

        for(auto platformEntity : platforms) {
            auto& platPos = platforms.get<Position>(platformEntity);
            auto& platSolid = platforms.get<Solid>(platformEntity);

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
        }
    }
}
