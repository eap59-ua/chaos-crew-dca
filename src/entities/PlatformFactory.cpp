#include <raylib.h>
#include "../entt/entt.hpp"
#include "../components/Platform.hpp"
#include "../components/Solid.hpp"
#include "../components/Position.hpp"
#include "../components/Velocity.hpp"
#include "../components/Sprite.hpp"
#include "../components/Velocity.hpp"
#include "../components/Trap.hpp"
#include "PlatformFactory.hpp"

entt::entity createPlatform(entt::registry& registry, float x, float y, float width, float height, float vx, float vy, Color color) {
    entt::entity platform = registry.create();
    
    // Componente de posición
    registry.emplace<Position>(platform, x, y);
    
    // Componente sólido para colisiones
    registry.emplace<Solid>(platform, width, height, color);

    // Componente de velocidad (estático) -> por si queremos que se mueva
    registry.emplace<Velocity>(platform, vx, vy);
    
    // Componente de plataforma
    registry.emplace<Platform>(platform);

    // Componente de sprite para renderizado
    registry.emplace<Sprite>(platform, Rectangle{ x, y, width, height });
    
    return platform;
}
