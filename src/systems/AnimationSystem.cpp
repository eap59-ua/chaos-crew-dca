#include <raylib.h>
#include "../entt/entt.hpp"
#include "../components/Sprite.hpp"
#include "../components/Velocity.hpp"
#include "../components/Player.hpp"
#include "../components/PlayerAnimations.hpp"
#include <cmath>

void AnimationSystem(entt::registry& registry, float dt) {
    
    // ---------------------------------------------------------
    // 1. LÓGICA ESPECÍFICA DEL JUGADOR (DECIDIR TEXTURA)
    // ---------------------------------------------------------
    // Esta parte se queda igual, solo quitamos el contador de tiempo al final.
    auto viewPlayers = registry.view<Sprite, Velocity, Player, PlayerAnimations>();

    for (auto entity : viewPlayers) {
        auto &sprite = viewPlayers.get<Sprite>(entity);
        auto &vel = viewPlayers.get<Velocity>(entity);
        auto &player = viewPlayers.get<Player>(entity);
        auto &anims = viewPlayers.get<PlayerAnimations>(entity);

        Texture2D targetTexture = anims.idle;
        int targetFrames = 11; 

        // ¿Hacia dónde mira?
        if (player.left) sprite.flipX = true;
        if (player.right) sprite.flipX = false;

        // Máquina de estados visual
        if (!player.onGround) {
            targetTexture = anims.jump;
            targetFrames = 1; 
        } 
        else if (std::abs(vel.vx) > 10.0f) { 
            targetTexture = anims.run;
            targetFrames = 12; 
        }
        else {
            targetTexture = anims.idle;
            targetFrames = 11;
        }

        // Si cambia la acción, reseteamos el sprite
        if (sprite.texture.id != targetTexture.id) {
            sprite.texture = targetTexture;
            sprite.totalFrames = targetFrames;
            sprite.currentFrame = 0;
            sprite.timer = 0;
        }
    }

    // ---------------------------------------------------------
    // 2. LÓGICA GENÉRICA DE AVANCE (JUGADORES + TRAMPAS)
    // ---------------------------------------------------------
    // Aquí iteramos sobre TODOS los sprites (incluyendo Wheel y Spikes).
    auto viewAllSprites = registry.view<Sprite>();

    for (auto entity : viewAllSprites) {
        auto &sprite = viewAllSprites.get<Sprite>(entity);

        // Solo procesamos si tiene animación (totalFrames > 1)
        // Esto hace que la rueda gire, pero los pinchos fijos no consuman CPU.
        if (sprite.totalFrames > 1) {
            sprite.timer += dt;
            
            // Usamos frameSpeed (o frameTime según como lo llamaste en el struct)
            if (sprite.timer >= sprite.frameTime) { 
                sprite.timer = 0.0f;
                sprite.currentFrame++;
                
                // Loop infinito
                if (sprite.currentFrame >= sprite.totalFrames) {
                    sprite.currentFrame = 0;
                }
            }
        }
    }
}