#include "SpikeFactory.hpp"
#include "../components/Position.hpp"
#include "../components/Solid.hpp"
#include "../components/Obstacle.hpp"
#include "../components/Velocity.hpp"
#include "../components/Sprite.hpp"
#include <cmath> // Necesario para roundf si queremos ser muy precisos, pero float va bien.

entt::entity createSpike(entt::registry& registry,
                         float mapX, float mapY, float mapWidth, float mapHeight, Texture2D texture)
{
    auto e = registry.create();

    float textureWidth = (float)texture.width;
    float textureHeight = (float)texture.height;

    // --- 1. CALCULAR ESCALA VISUAL ---
    // Escala base basada en el ancho del mapa
    float scale = mapWidth / textureWidth;
    
    // MULTIPLICADOR VISUAL: Aumenta esto para hacer el DIBUJO más grande.
    // Mantenemos el 2.5f que hace que se vean grandes como en tu imagen 3.
    float scaleMultiplier = 2.5f; 
    scale *= scaleMultiplier;

    // --- 2.RECORTAR LA HITBOX ---
    
    // Usar el 95% del ancho (para ajustar un poco los lados)
    float usableWidthRatio = 0.95f; 
    // Usaremos solo el 50% inferior de la altura (porque la mitad de arriba es aire)
    float usableHeightRatio = 0.50f; 

    // Calculamos las dimensiones finales de la caja verde basadas en estos ratios
    float finalHitboxWidth  = (textureWidth * scale) * usableWidthRatio;
    float finalHitboxHeight = (textureHeight * scale) * usableHeightRatio;

    // --- 3. CALCULAR POSICIÓN PARA CENTRAR ALINEADO ABAJO ---
    // Calculamos los offsets para meter nuestra hitbox ajustada dentro del área del mapa.

    // Offset Y: Alineamos la base de la hitbox con la base del área del mapa.
    float offsetY = mapHeight - finalHitboxHeight;

    // Offset X: Centramos horizontalmente.
    float offsetX = (mapWidth - finalHitboxWidth) / 2.0f;

    float finalX = mapX + offsetX;
    float finalY = mapY + offsetY;

    // --- 4. CREAR HITBOX ---
    // Usamos la poición y dimensiones AJUSTADAS.
    registry.emplace<Position>(e, finalX, finalY);
    registry.emplace<Solid>(e, finalHitboxWidth, finalHitboxHeight);
    registry.emplace<Velocity>(e, 0.0f, 0.0f);
    registry.emplace<Obstacle>(e);

    // --- 5. CREAR COMPONENTE SPRITE (DIBUJO) ---
    // El sprite se dibuja en la posición de la entidad. Como hemos movido la posición
    // para ajustar la hitbox, tenemos que "deshacer" ese movimiento visualmente usando
    // el offset del propio sprite para que el dibujo caiga exactamente sobre la hitbox.
    
    // Offset del Sprite = (Cuánto movimos la X, Cuánto movimos la Y + la parte transparente de arriba)
    float spriteOffsetX = offsetX + ((textureWidth * scale * (1.0f - usableWidthRatio)) / 2.0f);
    float spriteOffsetY = offsetY + (textureHeight * scale * (1.0f - usableHeightRatio));

    registry.emplace<Sprite>(e, 
        texture,
        Vector2{spriteOffsetX, spriteOffsetY}, // Offset visual correctivo
        textureWidth,
        textureHeight,
        scale,
        false, 1, 0, 0.0f, 0.0f
    );

    return e;
}