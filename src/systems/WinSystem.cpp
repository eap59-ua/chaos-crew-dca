#include "WinSystem.hpp"
#include "../components/Door.hpp"
#include "../components/Player.hpp"
#include "../components/Position.hpp"
#include "../components/Velocity.hpp"
#include "../components/Solid.hpp"
// #include "../components/Sprite.hpp" // Ya no lo necesitamos para la lógica
#include <iostream>
#include <raylib.h>

using namespace std;

bool CheckVictory(entt::registry& registry){
    // Obtenemos Door y Player CON sus componentes físicos (Position, Solid)
    auto doors = registry.view<Door, Position, Solid>();
    auto players = registry.view<Player, Position, Solid>();
    
    for (auto playerEntity : players) {
        auto &pPos = players.get<Position>(playerEntity);
        auto &pSolid = players.get<Solid>(playerEntity);
        
        // Creamos el rectángulo físico del jugador
        Rectangle playerRect = { pPos.x, pPos.y, pSolid.width, pSolid.height };

        bool playerTouchingAnyDoor = false;

        for (auto doorEntity : doors) {
            auto &dPos = doors.get<Position>(doorEntity);
            auto &dSolid = doors.get<Solid>(doorEntity);
            
            // Creamos el rectángulo físico de la puerta
            Rectangle doorRect = { dPos.x, dPos.y, dSolid.width, dSolid.height };

            if(CheckCollisionRecs(playerRect, doorRect)) {
                playerTouchingAnyDoor = true;
                break; 
            }
        }

        // Si este jugador NO está tocando ninguna puerta, no hay victoria aún
        if (!playerTouchingAnyDoor) return false;
    }

    // Si llegamos aquí, todos los jugadores están tocando alguna puerta
    return true;
}


bool CheckDefeat(entt::registry& registry){
    auto players = registry.view<Player, Position, Solid>();
    
    for (auto playerEntity : players) {
        auto &player = players.get<Player>(playerEntity);
        auto &pPos = players.get<Position>(playerEntity);
        auto &solid = players.get<Solid>(playerEntity);

        //Obstaculos mortales
        if(player.isDead) return true;
        
        // Caída al vacío
        if(pPos.y >= 720 - solid.height) return true;
    }

    return false;
}