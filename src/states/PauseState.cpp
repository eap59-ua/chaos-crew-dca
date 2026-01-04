#include "PauseState.hpp"
#include "GameplayState.hpp"
#include "../core/StateMachine.hpp"
#include "../locale/Locale.hpp"
#include <raylib.h>


void PauseState::handleInput() {
    
    // navegar
    if (IsKeyPressed(KEY_UP))    selected = (selected + 2) % 3; // rotar arriba
    if (IsKeyPressed(KEY_DOWN))  selected = (selected + 1) % 3; // rotar abajo

    // seleccionar con ENTER
    if (IsKeyPressed(KEY_ENTER)) {

        switch (selected) {
            
            // ✔ 1. CONTINUAR
            case 0:
                state_machine->remove_state(false); 
                break;
            
            // ✔ 2. REINICIAR NIVEL
            case 1:{

            
                std::string map = this->mapPath;

                state_machine->remove_state(false);
                state_machine->add_state(std::make_unique<GameplayState>(map), true);
                break;
            }

            // ✔ 3. SALIR DEL JUEGO
            case 2:
                state_machine->remove_state(true); 
                break;
        }
    }

    // opción rápida: ESC → volver
    if (IsKeyPressed(KEY_ESCAPE)) {
        state_machine->remove_state(false);
    }
}

void PauseState::render() {
    BeginDrawing();

    // Fondo semitransparente
    DrawRectangle(0, 0, GetScreenWidth(), GetScreenHeight(), Fade(BLACK, 0.6f));

    DrawText(_("PAUSA"), GetScreenWidth()/2 - MeasureText(_("PAUSA"), 50)/2, 150, 50, WHITE);

    const char* options[3] = { _("Continuar"), _("Reiniciar nivel"), _("Salir del juego") };

    for (int i = 0; i < 3; i++) {
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
