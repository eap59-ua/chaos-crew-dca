#include "core/StateMachine.hpp"
#include "states/GameplayState.hpp"
#include <chrono>
#include <memory>
#include <raylib.h>

int main() {
    const int SCREEN_WIDTH = 1280;
    const int SCREEN_HEIGHT = 720;
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Chaos Crew - Hito 1");
    SetTargetFPS(60);

    StateMachine state_machine;
    float delta_time = 0.0f;

    // Cargamos el primer estado (gameplay)
    state_machine.add_state(std::make_unique<GameplayState>(), false);
    state_machine.handle_state_changes(delta_time);

    while (!state_machine.is_game_ending() && !WindowShouldClose()) {
        delta_time = GetFrameTime();

        state_machine.handle_state_changes(delta_time);
        state_machine.getCurrentState()->handleInput();
        state_machine.getCurrentState()->update(delta_time);
        state_machine.getCurrentState()->render();
    }

    CloseWindow();
    return 0;
}
