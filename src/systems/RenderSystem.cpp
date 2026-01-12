#include <raylib.h>
#include <iostream>
#include "../entt/entt.hpp"
#include "../components/Player.hpp"
#include "../components/Platform.hpp"
#include "../components/Door.hpp"
#include "../components/Position.hpp"
#include "../components/Sprite.hpp"
#include "../components/Velocity.hpp"
#include "../components/Solid.hpp"
#include "../components/Obstacle.hpp"
#include "../components/Button.hpp"

#include "RenderSystem.hpp"

// Renderizado de BOTONES (Versión sin texturas)
void renderButtons(entt::registry& registry) {
    // Ya no buscamos Sprite ni Position (la posición visual ya está en btn.bounds)
    auto view = registry.view<Button>();

    for (auto entity : view) {
        const auto &btn = view.get<Button>(entity);

        // 1. Dibujar el relleno del botón (el color cambia si se pisa en ButtonSystem)
        DrawRectangleRec(btn.bounds, btn.color);

        // 2. (Opcional) Dibujar un borde negro para que se distinga mejor del fondo
        DrawRectangleLinesEx(btn.bounds, 2.0f, BLACK);
    }
}

// Renderizado de JUGADORES (Usa Sprite y Texturas)
void renderPlayers(entt::registry& registry) {
    auto view = registry.view<Player, Position, Sprite>();
    
    for (auto entity : view) {
        auto &pos = view.get<Position>(entity);
        auto &sprite = view.get<Sprite>(entity);

        // 1. SOURCE: Primer frame de la animación
        Rectangle sourceRec = { 
            0.0f, 
            0.0f, 
            sprite.flipX ? -sprite.frameWidth : sprite.frameWidth, 
            sprite.frameHeight 
        };

        // 2. DEST: Posición en pantalla con offset aplicado
        Rectangle destRec = {
            pos.x - sprite.offset.x,
            pos.y - sprite.offset.y,
            sprite.frameWidth * sprite.scale, 
            sprite.frameHeight * sprite.scale 
        };

        DrawTexturePro(sprite.texture, sourceRec, destRec, {0,0}, 0.0f, WHITE);
    }
}

// Renderizado de PLATAFORMAS (Usa Solid y Rectángulos simples)
void renderPlatforms(entt::registry& registry, Texture2D terrainTex) {
    auto platforms = registry.view<Platform, Position, Solid>();
    
    // Definimos qué tile usar. En "Terrain (32x32).png":
    // X=32, Y=0 suele ser un bloque de césped genérico muy útil.
    const float TILE_SIZE = 32.0f;
    Rectangle sourceRect = { 32.0f, 0.0f, TILE_SIZE, TILE_SIZE }; 

    for (auto platformEntity : platforms) {
        auto &pos = platforms.get<Position>(platformEntity);
        auto &solid = platforms.get<Solid>(platformEntity);
        
        // Calculamos cuántos tiles caben en la plataforma
        int tilesX = (int)(solid.width / TILE_SIZE);
        int tilesY = (int)(solid.height / TILE_SIZE);

        // Bucles anidados para rellenar el área
        for (int y = 0; y < tilesY; ++y) {
            for (int x = 0; x < tilesX; ++x) {
                Vector2 drawPos = {
                    pos.x + (x * TILE_SIZE),
                    pos.y + (y * TILE_SIZE)
                };
                
                // Dibujamos el trozo de textura
                DrawTextureRec(terrainTex, sourceRect, drawPos, WHITE);
            }
        }
    }
}

// Renderizado de TRAMPAS (Spike = triángulo rojo, Wheel = círculo rojo)
void renderTraps(entt::registry& registry) {
    auto traps = registry.view<Obstacle, Position, Solid>();

    for (auto entity : traps) {
        auto &pos = traps.get<Position>(entity);
        auto &solid = traps.get<Solid>(entity);

        if (registry.any_of<Sprite>(entity)) {
            auto &sprite = registry.get<Sprite>(entity);
            float sourceX = (sprite.totalFrames > 1) ? (sprite.currentFrame * sprite.frameWidth) : 0.0f; //evitar repetición
            Rectangle sourceRec = { sourceX, 0.0f, sprite.frameWidth, sprite.frameHeight };

            // CÁLCULO DE CENTRADO
            float destX = pos.x + (solid.width - (sprite.frameWidth * sprite.scale)) / 2.0f;
            float destY = pos.y + (solid.height - (sprite.frameHeight * sprite.scale)); // Base alineada

            // Si es la Rueda (animada), la centramos también verticalmente
            if (sprite.totalFrames > 1) {
                destY = pos.y + (solid.height - (sprite.frameHeight * sprite.scale)) / 2.0f;
            }

            Rectangle destRec = { destX, destY, sprite.frameWidth * sprite.scale, sprite.frameHeight * sprite.scale };
            DrawTexturePro(sprite.texture, sourceRec, destRec, {0,0}, 0.0f, WHITE);
        }
    }
}


// Renderizado de PUERTAS (Usa Solid y Rectángulos simples)
void renderDoors(entt::registry& registry, Texture2D doorTex) {
    auto doors = registry.view<Door, Position, Solid>();
    
    // La puerta de Pixel Adventure 1 mide 32x48 por frame.
    // Usamos el primer frame (puerta cerrada).
    const float doorVisualWidth = 48.0f;
    const float doorVisualHeight = 79.0f;
    Rectangle sourceRect = { 0.0f, 0.0f, doorVisualWidth, doorVisualHeight };

    for (auto doorEntity : doors) {
        auto &pos = doors.get<Position>(doorEntity);
        auto &solid = doors.get<Solid>(doorEntity);

        // Rectángulo de destino en pantalla con el tamaño visual real
        Rectangle destRect = {
            pos.x,
            pos.y,
            doorVisualWidth,
            doorVisualHeight
        };

        // ALINEACIÓN CLAVE:
        // Calculamos un offset para que la BASE de la puerta visual (48px de alto)
        // coincida exactamente con la BASE del hitbox físico (solid.height).
        // Esto permite que el hitbox físico sea más pequeño (ej: 32x32) y la puerta se dibuje alineada.
        Vector2 originOffset = { 
            0.0f, 
            doorVisualHeight - solid.height 
        };
        
        // Dibujamos usando el offset
        DrawTexturePro(doorTex, sourceRect, destRect, originOffset, 0.0f, WHITE);

        // --- OPCIONAL: Si quieres mantener el texto "EXIT" encima ---
        /*
        const char* text = "EXIT";
        int textWidth = MeasureText(text, 20);
        DrawText(text,
                (int)(pos.x + (doorVisualWidth - textWidth) / 2),
                (int)(pos.y - 25), // Dibujarlo 25px por encima de la puerta
                20,
                WHITE);
        */
    }
}

void renderScene(entt::registry& registry, Texture2D terrainTex, Texture2D doorTex) {

    renderPlatforms(registry, terrainTex);
    renderDoors(registry, doorTex); // Pasamos la textura de la puerta
    renderButtons(registry);
    
    renderPlayers(registry); 
    renderTraps(registry);
}