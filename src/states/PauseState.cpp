#include "PauseState.hpp"
#include "GameplayState.hpp"
#include "../core/StateMachine.hpp"
#include <raylib.h>

void PauseState::handleInput() {
    if (IsKeyPressed(KEY_P) || IsKeyPressed(KEY_ESCAPE)) {
        // Volver al gameplay
        state_machine->remove_state(false);
    }
}

void PauseState::render() {
    BeginDrawing();

    // Fondo semitransparente
    DrawRectangle(0, 0, GetScreenWidth(), GetScreenHeight(), Fade(BLACK, 0.6f));

    DrawText("PAUSED", GetScreenWidth()/2 - MeasureText("PAUSED", 60)/2, 
             GetScreenHeight()/2 - 100, 60, WHITE);

    DrawText("Press ESC or P to Resume", 
             GetScreenWidth()/2 - MeasureText("Press ESC or P to Resume", 30)/2,
             GetScreenHeight()/2, 30, GRAY);

    EndDrawing();
}
