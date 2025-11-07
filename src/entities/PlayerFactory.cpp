#include "../entt/entt.hpp"
#include "../components/Player.hpp"
#include "../components/Velocity.hpp"
#include "../components/Position.hpp"
#include "../components/Solid.hpp"
#include "../components/Sprite.hpp"
#include "../components/Mandos.hpp"
#include "PlayerFactory.hpp"

#include <raylib.h>

entt::entity createPlayer(entt::registry& registry, float x, float y, Color color, KeyboardKey LEFT, KeyboardKey RIGHT, KeyboardKey JUMP) {
    entt::entity player = registry.create();
    
    // Componente de posición
    registry.emplace<Position>(player, x, y);
    
    // Componente de velocidad
    registry.emplace<Velocity>(player, 20.0f, 1.0f);
    
    // Componente de jugador con teclas
    registry.emplace<Player>(player, false, false, false, false);
    
    // Componente sólido para colisiones
    registry.emplace<Solid>(player, 32.0f, 48.0f, color); // Ancho y alto del jugador

    // Inicializar sprite
    registry.emplace<Sprite>(player, Rectangle{x, y, 32.0f, 48.0f});

    // Configurar controles
    registry.emplace<Mandos>(player, LEFT, RIGHT, JUMP);
    
    return player;
}