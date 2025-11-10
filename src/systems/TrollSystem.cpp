#include "../components/Door.hpp"
#include "../components/Platform.hpp"
#include "../components/Player.hpp"
#include "../components/Position.hpp"
#include "../components/Solid.hpp"
#include "../components/Sprite.hpp"
#include "../components/Velocity.hpp"
#include "../components/Mandos.hpp"

#include "../components/Trap.hpp"
#include "../entt/entt.hpp"

void logicTroll(entt::registry& registry) {
    auto door = registry.view<Door, Position>();
    auto players = registry.view<Player, Position>();

    int condYes = 0;
    int payersCount = 0;

    for(auto playerEntity : players) {
        auto posPlayer = players.get<Position>(playerEntity);
        for(auto doorEntity : door) {
            auto posDoor = door.get<Position>(doorEntity);

            if(posDoor.x - 50 < posPlayer.x) condYes++;
        }
        payersCount++;
    }

    if(condYes == payersCount) {
        for (auto doorEntity : door) {
            auto& posDoor = door.get<Position>(doorEntity);
            posDoor.x = 40; // Mueve la puerta 10 unidades hacia la derecha
        }
    }
}