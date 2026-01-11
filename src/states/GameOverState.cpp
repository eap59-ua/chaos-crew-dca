#include "GameOverState.hpp"
#include "GameplayState.hpp"  // ✅ Incluir en .cpp, no en .hpp
#include "../utils/MapProgress.hpp"
#include "MainMenuState.hpp"
#include "../locale/Locale.hpp"

constexpr int SCREEN_WIDTH = 1280;
constexpr int SCREEN_HEIGHT = 720;

GameOverState::GameOverState(bool victory, std::string map)
    : levelCompleted(victory), mapPath(std::move(map)) {}

void GameOverState::handleInput() {

    if (IsKeyPressed(KEY_ENTER)) {

        auto prevGameplay = dynamic_cast<GameplayState*>( state_machine->getCurrentState().get() );
        std::string map = prevGameplay ? prevGameplay->getMapPath() : mapPath;

        // 👉 Si ganas, guardar progreso ANTES de volver al menú
        if (levelCompleted) {
            MapProgress::MarkCompleted(map);   // <--- AÑADE ESTA LÍNEA
            state_machine->add_state(std::make_unique<MainMenuState>(), true);
        }
        else {
            state_machine->add_state(std::make_unique<GameplayState>(map), true);
        }
    }
}

void GameOverState::render() {
    BeginDrawing();
    ClearBackground(BLACK);

    if (levelCompleted) {
        DrawText(_("LEVEL COMPLETE!"), SCREEN_WIDTH/2 - MeasureText(_("LEVEL COMPLETE!"), 60)/2, SCREEN_HEIGHT/2 - 100, 60, GREEN);
        DrawText(_("Press ENTER to return to MENU"), SCREEN_WIDTH/2 - 250, SCREEN_HEIGHT/2 + 40, 25, GRAY);
    } else {
        DrawText(_("GAME OVER"), SCREEN_WIDTH/2 - MeasureText(_("GAME OVER"), 60)/2, SCREEN_HEIGHT/2 - 100, 60, RED);
        DrawText(_("Press ENTER to retry"), SCREEN_WIDTH/2 - 160, SCREEN_HEIGHT/2 + 40, 25, GRAY);
    }

    EndDrawing();
}