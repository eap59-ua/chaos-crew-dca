#include <raylib.h>
#include "../entt/entt.hpp"
#include "../components/Player.hpp"
#include "../components/Velocity.hpp"
#include "../components/Position.hpp"
#include "../components/Solid.hpp"
#include "MovimentSystem.hpp"
#include "PhysicsConfig.hpp"

void MovementSystem(entt::registry& registry, float deltaTime, int SCREEN_WIDTH, int SCREEN_HEIGHT, Sound jumpSound) {
    auto players = registry.view<Player, Velocity, Position, Solid>();
    
    for (auto playerEntity : players) {
        auto &position = players.get<Position>(playerEntity);
        auto &solid = players.get<Solid>(playerEntity);
        auto &velocity = players.get<Velocity>(playerEntity);
        auto &player = players.get<Player>(playerEntity);
        
        // Dirección de entrada: -1 = izquierda, +1 = derecha
        float dir = (player.right ? 1.0f : 0.0f) - (player.left ? 1.0f : 0.0f);

        // Movimiento horizontal
        auto& physics = PhysicsConfig::Instance();
        float control = player.onGround ? 1.0f : physics.AIR_CONTROL;
        position.x += dir * physics.H_SPEED * control * deltaTime;

        // Limitar posición horizontal
        if (position.x < 0) {
            position.x = 0;
            velocity.vx = 0;
        } else if (position.x + solid.width > SCREEN_WIDTH) {
            position.x = SCREEN_WIDTH - solid.width;
            velocity.vx = 0;
        }

        // --- CORRECCIÓN AQUÍ ---
        // Aplicar gravedad SIEMPRE. No importa si está en el suelo.
        // Esto asegura que siempre haya una pequeña fuerza hacia abajo para mantener
        // el contacto con el suelo en el sistema de colisiones.
        velocity.vy += physics.GRAVITY * deltaTime;
        if (velocity.vy > physics.MAX_FALL_SPEED) velocity.vy = physics.MAX_FALL_SPEED;

        // Salto: sólo si onGround y el flag jump está activo
        if (player.jump && player.onGround) {
            velocity.vy = physics.JUMP_FORCE;
            player.onGround = false;
            PlaySound(jumpSound);
        }

        // Aplicar velocidad vertical
        position.y += velocity.vy * deltaTime;

        // Limitar posición vertical dentro de la pantalla
        if (position.y < 0) {
            position.y = 0;
            velocity.vy = 0;
        } else if (position.y + solid.height > SCREEN_HEIGHT) {
            position.y = SCREEN_HEIGHT - solid.height;
            velocity.vy = 0;
            player.onGround = true; 
        }
    }
}