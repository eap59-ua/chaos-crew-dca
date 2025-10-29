#include "core/StateMachine.hpp"
#include "states/GameplayState.hpp"
#include <raylib.h>

int main() {
    // Configuración de ventana
    const int SCREEN_WIDTH = 1280;
    const int SCREEN_HEIGHT = 720;
    
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Chaos Crew - Hito 1 Alpha");
    SetTargetFPS(60);
    
    // Crear máquina de estados
    StateMachine stateMachine;
    float deltaTime = 0.0f;
    
    // Cargar estado inicial (GameplayState)
    stateMachine.add_state(std::make_unique<GameplayState>(), false);
    stateMachine.handle_state_changes(deltaTime);
    
    // Bucle principal del juego
    while (!stateMachine.is_game_ending() && !WindowShouldClose()) {
        deltaTime = GetFrameTime();
        
        // Manejar cambios de estado
        stateMachine.handle_state_changes(deltaTime);
        
        // Obtener estado actual
        auto& currentState = stateMachine.getCurrentState();
        
        // Actualizar estado actual
        currentState->handleInput();
        currentState->update(deltaTime);
        currentState->render();
    }
    
    // Cleanup
    CloseWindow();
    
    return 0;
}