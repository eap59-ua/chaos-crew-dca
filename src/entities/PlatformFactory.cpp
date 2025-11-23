#include <raylib.h>
#include "../entt/entt.hpp"
#include "../components/Platform.hpp"
#include "../components/Solid.hpp"
#include "../components/Position.hpp"
#include "../components/Velocity.hpp"
// #include "../components/Sprite.hpp" <-- ELIMINADO: Ya no lo necesitamos aquí
#include "../components/Velocity.hpp"
#include "../components/Trap.hpp"
#include "PlatformFactory.hpp"

entt::entity createPlatform(entt::registry& registry, float x, float y, float width, float height, float vx, float vy, Color color) {
    entt::entity platform = registry.create();
    
    // Componente de posición
    registry.emplace<Position>(platform, x, y);
    
    // Componente sólido para colisiones (RenderSystem usará esto para saber el tamaño)
    registry.emplace<Solid>(platform, width, height, color);

    // Componente de velocidad (estático)
    registry.emplace<Velocity>(platform, vx, vy);
    
    // Componente de plataforma (Etiqueta)
    registry.emplace<Platform>(platform);

    // ❌ ELIMINADO: registry.emplace<Sprite>(platform, Rectangle{ x, y, width, height });
    // Las plataformas por ahora no tienen textura, se dibujan como rectángulos sólidos.
    
    return platform;
}