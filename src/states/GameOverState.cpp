#include "GameOverState.hpp"
#include "GameplayState.hpp"  // ✅ Incluir en .cpp, no en .hpp

constexpr int SCREEN_WIDTH = 1280;
constexpr int SCREEN_HEIGHT = 720;

GameOverState::GameOverState(bool victory)
    : levelCompleted(victory) {}

void GameOverState::handleInput() {
    if (IsKeyPressed(KEY_ENTER)) {
        state_machine->add_state(std::make_unique<GameplayState>(), true);
    }
}

void GameOverState::render() {
    BeginDrawing();
    ClearBackground(BLACK);

    if (levelCompleted) {
        DrawText("LEVEL COMPLETE!", SCREEN_WIDTH/2 - MeasureText("LEVEL COMPLETE!", 60)/2, SCREEN_HEIGHT/2 - 100, 60, GREEN);
        DrawText("Great teamwork!", SCREEN_WIDTH/2 - 150, SCREEN_HEIGHT/2 - 20, 30, WHITE);
    } else {
        DrawText("GAME OVER", SCREEN_WIDTH/2 - MeasureText("GAME OVER", 60)/2, SCREEN_HEIGHT/2 - 100, 60, RED);
        DrawText("One player fell!", SCREEN_WIDTH/2 - 150, SCREEN_HEIGHT/2 - 20, 30, WHITE);
    }

    DrawText("Press ENTER to retry", SCREEN_WIDTH/2 - 150, SCREEN_HEIGHT/2 + 50, 25, GRAY);
    DrawText("Chaos Crew - DCA UA 2025", SCREEN_WIDTH/2 - 150, SCREEN_HEIGHT - 50, 20, DARKGRAY);
    
    EndDrawing();
}