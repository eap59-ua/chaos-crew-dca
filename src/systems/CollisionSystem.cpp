#include <raylib.h>
#include "../entt/entt.hpp"
#include "../components/Obstacle.hpp"
#include "../components/Player.hpp"
#include "../components/Platform.hpp"
#include "../components/Position.hpp"
#include "../components/Velocity.hpp"
#include "../components/Solid.hpp"
#include "../components/Sprite.hpp"

void CollisionSystem(entt::registry& registry) {
    auto players   = registry.view<Player, Position, Velocity, Solid>();
    auto platforms = registry.view<Platform, Position, Solid>();
    float expand = 8.0f; //para expandir hitbox

    for (auto playerEntity : players) {
        auto& player = players.get<Player>(playerEntity);
        auto& pos    = players.get<Position>(playerEntity);
        auto& vel    = players.get<Velocity>(playerEntity);
        auto& solid  = players.get<Solid>(playerEntity);

        // Reset ground state
        player.onGround = false;

        // ===============================
        // 1. COLISIONES CON PLATAFORMAS
        // ===============================
        for (auto platformEntity : platforms) {
            auto& platPos   = platforms.get<Position>(platformEntity);
            auto& platSolid = platforms.get<Solid>(platformEntity);

            Rectangle playerRect   = { pos.x, pos.y, solid.width, solid.height };
            Rectangle platformRect = { platPos.x, platPos.y, platSolid.width, platSolid.height };

            if (CheckCollisionRecs(playerRect, platformRect)) {
                // Calcular solapamientos
                float overlapTop    = (platPos.y + platSolid.height) - pos.y;
                float overlapBottom = (pos.y + solid.height) - platPos.y;
                float overlapLeft   = (platPos.x + platSolid.width) - pos.x;
                float overlapRight  = (pos.x + solid.width) - platPos.x;

                // Encontrar la menor penetración
                float minOverlap = fminf(fminf(overlapTop, overlapBottom),
                                         fminf(overlapLeft, overlapRight));

                // Resolver colisión según penetración mínima
                if (minOverlap == overlapBottom && vel.vy >= 0) {
                    // Colisión desde arriba
                    pos.y = platPos.y - solid.height;
                    vel.vy = 0;
                    player.onGround = true;
                }
                else if (minOverlap == overlapTop && vel.vy <= 0) {
                    // Desde abajo
                    pos.y = platPos.y + platSolid.height;
                    vel.vy = 0;
                }
                else if (minOverlap == overlapLeft && vel.vx <= 0) {
                    // Derecha
                    pos.x = platPos.x + platSolid.width;
                    vel.vx = 0;
                }
                else if (minOverlap == overlapRight && vel.vx >= 0) {
                    // Izquierda
                    pos.x = platPos.x - solid.width;
                    vel.vx = 0;
                }
            }
        }

        // ============================
        // 2. COLISIONES CON TRAMPAS
        // ============================

        auto obstacles = registry.view<Obstacle, Position, Solid>();

        for (auto obs : obstacles) {
            auto& oPos = obstacles.get<Position>(obs);
            auto& oSolid = obstacles.get<Solid>(obs);

            Rectangle playerRect = { pos.x, pos.y, solid.width, solid.height };
            
            Rectangle trapRect   = { 
                oPos.x, 
                oPos.y, 
                oSolid.width, 
                oSolid.height 
            };

            if (CheckCollisionRecs(playerRect, trapRect)) {

                // ¿Es rueda? (ancho ≈ alto)
                bool isWheel = fabs(oSolid.width - oSolid.height) < 1.0f;

                if (isWheel) {
                    // MUERE EN CUALQUIER CONTACTO
                    player.isDead = true;
                    continue;
                }

                // ============================
                //          ES PINCHO
                // ============================

                bool comingFromAbove = (pos.y + solid.height) <= (oPos.y + 5.0f);

                // SI VIENE DESDE ARRIBA → MUERE
                if (comingFromAbove) {
                    player.isDead = true;
                }

                // SI NO → COLISIÓN SÓLIDA (bloquear)
                // calculamos igual que plataformas
                float overlapTop = (oPos.y + oSolid.height) - pos.y;
                float overlapBottom = (pos.y + solid.height) - oPos.y;
                float overlapLeft = (oPos.x + oSolid.width) - pos.x;
                float overlapRight = (pos.x + solid.width) - oPos.x;

                float minOverlap = fminf(fminf(overlapTop, overlapBottom),
                                        fminf(overlapLeft, overlapRight)); // mínima penetración: dirección de la colisión

                if (minOverlap == overlapBottom && vel.vy >= 0) {
                    pos.y = oPos.y - solid.height;
                    vel.vy = 0;
                    player.onGround = true;
                }
                else if (minOverlap == overlapTop && vel.vy <= 0) {
                    pos.y = oPos.y + oSolid.height;
                    vel.vy = 0;
                }
                else if (minOverlap == overlapLeft && vel.vx <= 0) {
                    pos.x = oPos.x + oSolid.width;
                    vel.vx = 0;
                }
                else if (minOverlap == overlapRight && vel.vx >= 0) {
                    pos.x = oPos.x - solid.width;
                    vel.vx = 0;
                }
            }
        }
    }
}
