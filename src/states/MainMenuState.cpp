#include "MainMenuState.hpp"
#include "GameplayState.hpp"
#include "../utils/MapProgress.hpp"
#include "../locale/Locale.hpp"

#include <raylib.h>
#include <algorithm>

static constexpr int SCREEN_W = 1280;
static constexpr int SCREEN_H = 720;

void MainMenuState::init(){
    rebuildData();
    PlayMusicStream(menuMusic);
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

    DrawText(_("CHAOS CREW"), SCREEN_W/2 - MeasureText(_("CHAOS CREW"), 60)/2, 60, 60, YELLOW);
    DrawText(_("Choose a Level (UP/DOWN, ENTER). R = refresh"), SCREEN_W/2 - 300, 130, 20, GRAY);

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
            DrawText(_("[LOCKED]"), x + 520, y, 28, RED);
        } else if (completed[i]) {
            DrawText(_("[DONE]"), x + 520, y, 28, GREEN);
        }
    }

    if (maps.empty()) {
        DrawText(_("No maps found in ./mapas"), SCREEN_W/2 - 180, SCREEN_H/2, 28, RED);
    }

    // Display connected gamepads at bottom right
    int gamepadCount = 0;
    const int gamepadStartY = SCREEN_H - 150;

    DrawText(_("CONTROLLERS:"), SCREEN_W - 300, gamepadStartY - 30, 16, GRAY);
    DrawText(_("P1: Keyboard (Arrows)"), SCREEN_W - 300, gamepadStartY, 14, RAYWHITE);
    DrawText(_("P2: Keyboard (WASD)"), SCREEN_W - 300, gamepadStartY + 20, 14, RAYWHITE);

    for (int i = 0; i < 4; i++) {
        if (IsGamepadAvailable(i)) {
            const char* gamepadName = GetGamepadName(i);
            const char* displayText = TextFormat(_("P%d: %s"), gamepadCount + 3, gamepadName);
            DrawText(displayText, SCREEN_W - 300, gamepadStartY + 40 + (gamepadCount * 20), 14, GREEN);
            gamepadCount++;
        }
    }

    if (gamepadCount == 0) {
        DrawText(_("(No gamepads detected)"), SCREEN_W - 300, gamepadStartY + 40, 14, DARKGRAY);
    }

    EndDrawing();
}
