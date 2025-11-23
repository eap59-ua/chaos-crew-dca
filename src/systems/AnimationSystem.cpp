#include <raylib.h>
#include "../entt/entt.hpp"
#include "../components/Sprite.hpp"
#include "../components/Velocity.hpp"
#include "../components/Player.hpp"
#include "../components/PlayerAnimations.hpp"
#include <cmath>

void AnimationSystem(entt::registry& registry, float dt) {
    auto view = registry.view<Sprite, Velocity, Player, PlayerAnimations>();

    for (auto entity : view) {
        auto &sprite = view.get<Sprite>(entity);
        auto &vel = view.get<Velocity>(entity);
        auto &player = view.get<Player>(entity);
        auto &anims = view.get<PlayerAnimations>(entity);

        // 1. LÓGICA DE CAMBIO DE ESTADO (Cambiar Textura)
        Texture2D targetTexture = anims.idle;
        int targetFrames = 11; // Ajusta esto según tus imágenes (Pixel Adventure Idle suele ser 11 frames)

        // ¿Hacia dónde mira?
        if (player.left) sprite.flipX = true;
        if (player.right) sprite.flipX = false;

        if (!player.onGround) {
            // ESTÁ EN EL AIRE (Salto)
            targetTexture = anims.jump;
            targetFrames = 1; // El salto suele ser 1 frame o pocos
        } 
        else if (std::abs(vel.vx) > 10.0f) { // Usamos un umbral pequeño
            // ESTÁ CORRIENDO
            targetTexture = anims.run;
            targetFrames = 12; // Pixel Adventure Run suele ser 12 frames
        }
        else {
            // ESTÁ QUIETO (Idle)
            targetTexture = anims.idle;
            targetFrames = 11;
        }

        // Si la textura cambia, reseteamos la animación
        if (sprite.texture.id != targetTexture.id) {
            sprite.texture = targetTexture;
            sprite.totalFrames = targetFrames;
            sprite.currentFrame = 0;
            sprite.timer = 0;
        }

        // 2. AVANCE DE FRAMES (Loop de animación)
        sprite.timer += dt;
        if (sprite.timer >= sprite.frameTime) {
            sprite.timer = 0.0f;
            sprite.currentFrame++;
            
            // Si llegamos al final, volvemos al principio (Loop)
            if (sprite.currentFrame >= sprite.totalFrames) {
                sprite.currentFrame = 0;
            }
        }
    }
}