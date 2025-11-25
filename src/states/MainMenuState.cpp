#include "MainMenuState.hpp"
#include "GameplayState.hpp"
#include "../utils/MapProgress.hpp"

#include <raylib.h>
#include <algorithm>

static constexpr int SCREEN_W = 1280;
static constexpr int SCREEN_H = 720;

void MainMenuState::init(){
    rebuildData();
}

void MainMenuState::rebuildData(){
    maps = MapProgress::GetAvailableMaps();
    auto done = MapProgress::Load();

    completed.assign(maps.size(), false);
    unlocked.assign(maps.size(), false);

    // Regla: el primero siempre desbloqueado; el resto se desbloquea si el anterior está completado
    for (size_t i = 0; i < maps.size(); ++i) {
        completed[i] = done.count(maps[i]) > 0;
        if (i == 0) unlocked[i] = true;
        else        unlocked[i] = completed[i-1]; // progresión lineal
    }

    if (selected >= (int)maps.size()) selected = (int)maps.size() - 1;
    if (selected < 0) selected = 0;
}

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
    if (IsKeyPressed(KEY_DOWN))  { selected = (selected + 1) % (int)maps.size(); }
    if (IsKeyPressed(KEY_UP))    { selected = (selected - 1 + (int)maps.size()) % (int)maps.size(); }

    if (IsKeyPressed(KEY_ENTER) && !maps.empty()) {
        if (unlocked[selected]) {
            state_machine->add_state(std::make_unique<GameplayState>(maps[selected]), true);
        }
    }

    // Por si el archivo de progreso cambia externamente (no es obligatorio)
    if (IsKeyPressed(KEY_R)) rebuildData();
}

void MainMenuState::render() {
    BeginDrawing();
    ClearBackground(BLACK);

    DrawText("CHAOS CREW", SCREEN_W/2 - MeasureText("CHAOS CREW", 60)/2, 60, 60, YELLOW);
    DrawText("Choose a Level (UP/DOWN, ENTER). R = refresh", SCREEN_W/2 - 300, 130, 20, GRAY);

    int startY = 200;
    for (int i = 0; i < (int)maps.size(); ++i) {
        const bool isSel = (i == selected);
        const char* name = maps[i].c_str(); // puedes recortar "mapas/" + ".xml" si quieres

        int x = 300;
        int y = startY + i * 40;

        if (isSel) DrawRectangle(x-10, y-6, 700, 34, Fade(WHITE, 0.1f));

        Color textColor = unlocked[i] ? RAYWHITE : GRAY;
        DrawText(name, x, y, 28, textColor);

        if (!unlocked[i]) {
            DrawText("[LOCKED]", x + 520, y, 28, RED);
        } else if (completed[i]) {
            DrawText("[DONE]", x + 520, y, 28, GREEN);
        }
    }

    if (maps.empty()) {
        DrawText("No maps found in ./mapas", SCREEN_W/2 - 180, SCREEN_H/2, 28, RED);
    }

    EndDrawing();
}
