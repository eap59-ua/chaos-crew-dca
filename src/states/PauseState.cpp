#include "PauseState.hpp"
#include "GameplayState.hpp"
#include "MainMenuState.hpp" // <--- AÑADE ESTO (Ajusta el nombre si tu archivo es distinto)
#include "../core/StateMachine.hpp"
#include "../locale/Locale.hpp"
#include <raylib.h>

void PauseState::handleInput() {
    
    // Navegar (Ahora son 4 opciones, así que usamos % 4)
    // Para ir hacia arriba: (actual + total_opciones - 1) % total_opciones
    // (selected + 3) % 4 es equivalente a ir uno hacia atrás en un ciclo de 4.
    if (IsKeyPressed(KEY_UP))    selected = (selected + 3) % 4; // <--- CAMBIADO
    if (IsKeyPressed(KEY_DOWN))  selected = (selected + 1) % 4; // <--- CAMBIADO

    // Seleccionar con ENTER
    if (IsKeyPressed(KEY_ENTER)) {

        switch (selected) {
            
            // 0. CONTINUAR
            case 0:
                state_machine->remove_state(false); 
                break;

            // 1. VOLVER AL MENÚ (NUEVA OPCIÓN)
            case 1:
                // 'true' en add_state suele significar "reemplazar todo" o "limpiar pila"
                // dependiendo de tu implementación de StateMachine.
                state_machine->add_state(std::make_unique<MainMenuState>(), true); 
                break;
            
            // 2. REINICIAR NIVEL (Desplazado)
            case 2:{
                std::string map = this->mapPath;
                // Reiniciamos reemplazando la pila con un nuevo GameplayState
                state_machine->add_state(std::make_unique<GameplayState>(map), true);
                break;
            }

            // 3. SALIR DEL JUEGO (Desplazado)
            case 3:
                state_machine->remove_state(true); // Asumo que 'true' aquí cierra la app en tu lógica
                break;
        }
    }

    // Opción rápida: ESC -> volver (Continuar)
    if (IsKeyPressed(KEY_ESCAPE)) {
        state_machine->remove_state(false);
    }
}

void PauseState::render() {
    BeginDrawing();

    // Fondo semitransparente
    DrawRectangle(0, 0, GetScreenWidth(), GetScreenHeight(), Fade(BLACK, 0.6f));

    DrawText(_("PAUSA"), GetScreenWidth()/2 - MeasureText(_("PAUSA"), 50)/2, 150, 50, WHITE);

    // Ahora son 4 opciones
    const char* options[4] = { 
        _("Continuar"), 
        _("Volver al menú"),  // <--- NUEVA OPCIÓN
        _("Reiniciar nivel"), 
        _("Salir del juego") 
    };

    for (int i = 0; i < 4; i++) { // <--- i < 4
        Color color = (i == selected) ? YELLOW : RAYWHITE;
        DrawText(
            options[i],
            GetScreenWidth()/2 - MeasureText(options[i], 30)/2,
            260 + i * 60,
            30,
            color
        );
    }

    EndDrawing();
}