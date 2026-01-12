#include "ButtonFactory.hpp"
#include "../components/Position.hpp"
#include "../components/Solid.hpp"
#include "../components/Button.hpp"
// #include "../components/Sprite.hpp" <--- YA NO LO NECESITAMOS

// Nota: He quitado 'Texture2D texture' de los argumentos porque ya no se usa.
entt::entity createButton(entt::registry& registry, float x, float y, int channel) {
    auto entity = registry.create();

    // 1. POSICIÓN LÓGICA
    // (x, y) es la esquina superior izquierda de la hitbox física (suelo)
    registry.emplace<Position>(entity, x, y);

    // 2. HITBOX FÍSICA (Solid)
    // Caja de 32 ancho x 10 alto. El jugador camina sobre esto.
    registry.emplace<Solid>(entity, 32.0f, 10.0f, RED);

    // 3. COMPONENTE DE BOTÓN (Lógica + Visual)
    // Calculamos dónde dibujar el cuadrado visual.
    // Como la hitbox mide 10 de alto y queremos un botón visual de 32 de alto,
    // tenemos que "subir" el dibujo 22 píxeles para que la base coincida.
    
    Rectangle visualRect = {
        x,              // Misma X que la física
        y - 22.0f,      // Subimos 22px hacia arriba
        32.0f,          // Ancho visual
        32.0f           // Alto visual
    };

    registry.emplace<Button>(
        entity, 
        channel, 
        false,          // isPressed
        false,          // executed
        nullptr,        // onPressAction
        visualRect,     // bounds (El rectángulo visual)
        BLUE            // color (Color inicial sin pisar)
    );

    return entity;
}