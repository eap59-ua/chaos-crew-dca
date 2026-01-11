#include "../entt/entt.hpp"
#include "../components/Player.hpp"
#include "../components/Velocity.hpp"
#include "../components/Position.hpp"
#include "../components/Solid.hpp"
#include "../components/Sprite.hpp"
#include "../components/Mandos.hpp"
#include "PlayerFactory.hpp"

#include <raylib.h>

// CAMBIO: Ahora recibimos 'PlayerAnimations anims' en lugar de una sola textura
entt::entity createPlayer(entt::registry& registry, float x, float y, PlayerAnimations anims, KeyboardKey LEFT, KeyboardKey RIGHT, KeyboardKey JUMP) {
    entt::entity player = registry.create();
    
    // Componente de posición
    registry.emplace<Position>(player, x, y);
    
    // Componente de velocidad
    registry.emplace<Velocity>(player, 0.0f, 0.0f);
    
    // Componente de jugador
    registry.emplace<Player>(player, false, false, false, false);
    
    // 1. Definimos el tamaño físico (Hitbox)
    // El hitbox es más pequeño que el sprite para que sea más justo
    float hitboxWidth = 36.0f;  
    float hitboxHeight = 50.0f;

    registry.emplace<Solid>(player, hitboxWidth, hitboxHeight, WHITE);

    // --- NUEVO: Guardar todas las animaciones en la entidad ---
    // Esto permite que el AnimationSystem cambie entre Idle, Run y Jump
    registry.emplace<PlayerAnimations>(player, anims);

    // 2. Configuración del Sprite Inicial
    float spriteSize = 32.0f; // Tamaño del tile original (32x32)
    float scale = 2.0f;       // Escala visual
    
    // Calculamos el offset para centrar la imagen sobre el hitbox
    Vector2 visualOffset;
    visualOffset.x = (spriteSize * scale - hitboxWidth) / 2.0f; 
    visualOffset.y = (spriteSize * scale - hitboxHeight);       
    
    // Crear el componente Sprite
    // Empezamos usando la animación IDLE por defecto
    registry.emplace<Sprite>(player, 
                             anims.idle,    // Textura inicial (Idle)
                             visualOffset,  
                             spriteSize,    // frameWidth
                             spriteSize,    // frameHeight
                             scale,         // escala
                             false,         // flipX
                             11,            // totalFrames (Idle tiene 11 frames)
                             0,             // currentFrame (empieza en 0)
                             0.05f,         // frameTime (velocidad de animación)
                             0.0f           // timer interno
                             );
    
    // Configurar controles
    registry.emplace<Mandos>(player, InputType::KEYBOARD, LEFT, RIGHT, JUMP, 0, 0.15f);

    return player;
}

entt::entity createPlayerWithGamepad(entt::registry& registry, float x, float y, PlayerAnimations anims, int gamepadIndex) {
    entt::entity player = registry.create();

    // Componente de posición
    registry.emplace<Position>(player, x, y);

    // Componente de velocidad
    registry.emplace<Velocity>(player, 0.0f, 0.0f);

    // Componente de jugador
    registry.emplace<Player>(player, false, false, false, false);

    // Hitbox
    float hitboxWidth = 36.0f;
    float hitboxHeight = 50.0f;
    registry.emplace<Solid>(player, hitboxWidth, hitboxHeight, WHITE);

    // Animaciones
    registry.emplace<PlayerAnimations>(player, anims);

    // Configuración del Sprite Inicial
    float spriteSize = 32.0f;
    float scale = 2.0f;

    Vector2 visualOffset;
    visualOffset.x = (spriteSize * scale - hitboxWidth) / 2.0f;
    visualOffset.y = (spriteSize * scale - hitboxHeight);

    registry.emplace<Sprite>(player,
                             anims.idle,
                             visualOffset,
                             spriteSize,
                             spriteSize,
                             scale,
                             false,
                             11,
                             0,
                             0.05f,
                             0.0f
                             );

    // Configurar controles de gamepad
    registry.emplace<Mandos>(player, InputType::GAMEPAD, KEY_NULL, KEY_NULL, KEY_NULL, gamepadIndex, 0.15f);

    return player;
}