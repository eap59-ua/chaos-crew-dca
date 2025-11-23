#include "MainMenuState.hpp"
#include "GameplayState.hpp"
#include "../core/StateMachine.hpp"
#include <raylib.h>

MainMenuState::MainMenuState() {
    // Cargar música del menú (ej: una canción más tranquila)
    menuMusic = LoadMusicStream("assets/sounds/Theme.wav");
}

MainMenuState::~MainMenuState() {
    UnloadMusicStream(menuMusic);
}

void MainMenuState::init() {
    PlayMusicStream(menuMusic);
}

void MainMenuState::update(float dt) {
    UpdateMusicStream(menuMusic);
}

void MainMenuState::handleInput() {
    if (IsKeyPressed(KEY_ENTER)) {
        state_machine->add_state(std::make_unique<GameplayState>(), true);
    }
}

void MainMenuState::render() {
    BeginDrawing();
    ClearBackground(BLACK);

    DrawText("CHAOS CREW", 
             GetScreenWidth()/2 - MeasureText("CHAOS CREW", 60)/2,
             150, 60, YELLOW);

    DrawText("Press ENTER to Play", 
             GetScreenWidth()/2 - MeasureText("Press ENTER to Play", 30)/2,
             350, 30, RAYWHITE);

    EndDrawing();
}
