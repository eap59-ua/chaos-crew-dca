#include "states/GameplayState.hpp"
#include "states/GameOverState.hpp"
#include "core/StateMachine.hpp"

constexpr int SCREEN_WIDTH = 1280;
constexpr int SCREEN_HEIGHT = 720;
constexpr float GRAVITY = 0.6f;
constexpr float JUMP_FORCE = -12.0f;
constexpr float MOVE_SPEED = 4.5f;

GameplayState::GameplayState() 
    : levelCompleted(false), isGameOver(false) {}

void GameplayState::init() {
    // Jugador 1
    player1.rect = {100, SCREEN_HEIGHT - 200, 30, 40};
    player1.velocity = {0, 0};
    player1.color = BLUE;
    player1.isGrounded = false;
    player1.leftKey = KEY_A;
    player1.rightKey = KEY_D;
    player1.jumpKey = KEY_W;

    // Jugador 2
    player2.rect = {150, SCREEN_HEIGHT - 200, 30, 40};
    player2.velocity = {0, 0};
    player2.color = RED;
    player2.isGrounded = false;
    player2.leftKey = KEY_LEFT;
    player2.rightKey = KEY_RIGHT;
    player2.jumpKey = KEY_UP;

    // Plataformas
    platforms.clear();
    platforms.push_back({{0, SCREEN_HEIGHT - 50, SCREEN_WIDTH, 50}, DARKGRAY});
    platforms.push_back({{200, SCREEN_HEIGHT - 200, 200, 20}, GRAY});
    platforms.push_back({{500, SCREEN_HEIGHT - 300, 200, 20}, GRAY});
    platforms.push_back({{800, SCREEN_HEIGHT - 400, 200, 20}, GRAY});
    platforms.push_back({{1000, SCREEN_HEIGHT - 250, 150, 20}, GRAY});

    exitZone = {SCREEN_WIDTH - 100, SCREEN_HEIGHT - 150, 80, 100};

    levelCompleted = false;
    isGameOver = false;
}

void GameplayState::handleInput() {
    // Reinicio manual si se desea
    if (IsKeyPressed(KEY_ENTER)) {
        init();
    }
}

void GameplayState::update(float deltaTime) {
    auto updatePlayer = [&](Player& p) {
        // Movimiento horizontal
        p.velocity.x = 0;
        if (IsKeyDown(p.leftKey)) p.velocity.x = -MOVE_SPEED;
        if (IsKeyDown(p.rightKey)) p.velocity.x = MOVE_SPEED;

        // Salto
        if (IsKeyPressed(p.jumpKey) && p.isGrounded) {
            p.velocity.y = JUMP_FORCE;
            p.isGrounded = false;
        }

        // Gravedad
        if (!p.isGrounded) p.velocity.y += GRAVITY;

        // Movimiento
        p.rect.x += p.velocity.x;
        p.rect.y += p.velocity.y;

        // Límites
        if (p.rect.x < 0) p.rect.x = 0;
        if (p.rect.x + p.rect.width > SCREEN_WIDTH)
            p.rect.x = SCREEN_WIDTH - p.rect.width;

        // Colisiones con plataformas
        p.isGrounded = false;
        for (auto& plat : platforms) {
            if (CheckCollisionRecs(p.rect, plat.rect) && p.velocity.y > 0) {
                p.rect.y = plat.rect.y - p.rect.height;
                p.velocity.y = 0;
                p.isGrounded = true;
            }
        }

        // Muerte
        if (p.rect.y > SCREEN_HEIGHT) isGameOver = true;
    };

    updatePlayer(player1);
    updatePlayer(player2);

    if (isGameOver) {
        state_machine->add_state(std::make_unique<GameOverState>(false), true);
        return;
    }

    // Condición de victoria
    bool p1AtExit = CheckCollisionRecs(player1.rect, exitZone);
    bool p2AtExit = CheckCollisionRecs(player2.rect, exitZone);

    if (p1AtExit && p2AtExit) {
        state_machine->add_state(std::make_unique<GameOverState>(true), true);
    }
}

void GameplayState::render() {
    BeginDrawing();
    ClearBackground(RAYWHITE);

    // Plataformas
    for (auto& plat : platforms) DrawRectangleRec(plat.rect, plat.color);

    // Zona de salida
    DrawRectangleRec(exitZone, GREEN);
    DrawText("EXIT", exitZone.x + 20, exitZone.y + 40, 20, DARKGREEN);

    // Jugadores
    DrawRectangleRec(player1.rect, player1.color);
    DrawText("P1", player1.rect.x + 5, player1.rect.y + 10, 20, WHITE);

    DrawRectangleRec(player2.rect, player2.color);
    DrawText("P2", player2.rect.x + 5, player2.rect.y + 10, 20, WHITE);

    // HUD
    DrawRectangle(0, 0, SCREEN_WIDTH, 100, Fade(BLACK, 0.7f));
    DrawText("CHAOS CREW - HITO 1", 20, 10, 30, YELLOW);
    DrawText("P1: A/D/W | P2: Arrows", 20, 45, 20, GRAY);
    EndDrawing();
}
