#include <raylib.h>
#include "../entt/entt.hpp"
#include "../components/Door.hpp"
#include "../components/Position.hpp"
#include "../components/Solid.hpp"
#include "../components/Sprite.hpp"
#include "DoorFactory.hpp"

entt::entity createDoor(entt::registry& registry, float x, float y, float width, float height, Color color) {
    entt::entity doorEntity = registry.create();
    
    // Añadir componente Position
    registry.emplace<Position>(doorEntity, x, y);
    
    // Añadir componente Solid
    registry.emplace<Solid>(doorEntity, width, height, color);
    
    // Añadir componente Door con el nivel objetivo
    registry.emplace<Door>(doorEntity);

    // Añadir componente Sprite para renderizado
   // registry.emplace<Sprite>(doorEntity, Rectangle{ x, y, width, height });
    
    return doorEntity;
}