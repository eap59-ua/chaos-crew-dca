#include <raylib.h>
#include "../entt/entt.hpp"
#include "../components/Player.hpp"
#include "../components/Velocity.hpp"
#include "../components/Position.hpp"
#include "../components/Solid.hpp"
#include "MovimentSystem.hpp"

// Tunables: ajustar para el "feel"
static constexpr float H_SPEED = 360.0f;      // velocidad horizontal (px/s)
static constexpr float AIR_CONTROL = 0.85f;   // control horizontal en el aire (0..1)
static constexpr float GRAVITY = 700.0f;     // gravedad (px/s^2)
static constexpr float JUMP_FORCE = -700.0f;  // fuerza de salto (negativa hacia arriba)
static constexpr float MAX_FALL_SPEED = 1200.0f; // velocidad máxima de caída (px/s)

void MovementSystem(entt::registry& registry, float deltaTime, int SCREEN_WIDTH, int SCREEN_HEIGHT) {
    auto players = registry.view<Player, Velocity, Position, Solid>();
    
    for (auto playerEntity : players) {
    	auto &position = players.get<Position>(playerEntity);
    	auto &solid = players.get<Solid>(playerEntity);
    	auto &velocity = players.get<Velocity>(playerEntity);
    	auto &player = players.get<Player>(playerEntity);
    	
    	// Dirección de entrada: -1 = izquierda, +1 = derecha
        float dir = (player.right ? 1.0f : 0.0f) - (player.left ? 1.0f : 0.0f);

        // Movimiento horizontal inmediato (mejor respuesta)
        float control = player.onGround ? 1.0f : AIR_CONTROL;
        position.x += dir * H_SPEED * control * deltaTime;

        // Limitar posición horizontal dentro de la pantalla
        if (position.x < 0) {
            position.x = 0;
            velocity.vx = 0;
        } else if (position.x + solid.width > SCREEN_WIDTH) {
            position.x = SCREEN_WIDTH - solid.width;
            velocity.vx = 0;
        }

        // Aplicar gravedad si no está en suelo
        if (!player.onGround) {
            velocity.vy += GRAVITY * deltaTime;
            if (velocity.vy > MAX_FALL_SPEED) velocity.vy = MAX_FALL_SPEED;
        } else {
            // asegurar que no acumule velocidad positiva al estar en suelo
            if (velocity.vy > 0.0f) velocity.vy = 0.0f;
        }

        // Salto: sólo si onGround y el flag jump está activo
        if (player.jump && player.onGround) {
            velocity.vy = JUMP_FORCE;
            player.onGround = false;
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
            player.onGround = true; // Considerar el suelo de la pantalla como superficie
        }
	}
}
