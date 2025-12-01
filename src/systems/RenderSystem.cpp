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

#include "RenderSystem.hpp"

// Renderizado de JUGADORES (Usa Sprite y Texturas)
void renderPlayers(entt::registry& registry) {
    auto view = registry.view<Position, Sprite>();
    
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
    
    auto traps = registry.view<Obstacle, Position, Solid>(); // Solo entidades con Trap, Position y Solid


    for (auto trapsEntity : traps) {
        auto &pos = traps.get<Position>(trapsEntity);
        auto &solid   = traps.get<Solid>(trapsEntity);

        // Si la trampa es casi cuadrada → la consideramos "Wheel"
        if (fabs(solid.width - solid.height) < 1.0f) {
            float radius = solid.width / 2.0f;
            DrawCircle(pos.x + radius, pos.y + radius, radius, RED);
        }
        else {
            // Trampa Spike → triángulo rojo
            float lado = solid.width;                       // el tamaño real del triángulo
            float h = (sqrtf(3) / 2.0f) * lado;             // altura de un triángulo equilátero

            // pos = centro del triángulo
            Vector2 p1 = { pos.x, pos.y - (2.0f/3.0f) * h }; // vértice superior
            Vector2 p2 = { pos.x - lado/2.0f, pos.y + (1.0f/3.0f) * h }; // inferior izquierda
            Vector2 p3 = { pos.x + lado/2.0f, pos.y + (1.0f/3.0f) * h }; // inferior derecha

            DrawTriangle(p1, p2, p3, RED);
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
    std::cout << "[RenderScene] registry=" << &registry << std::endl;

    renderPlatforms(registry, terrainTex);
    renderDoors(registry, doorTex); // Pasamos la textura de la puerta
    
    renderPlayers(registry); 
    renderTraps(registry);
}