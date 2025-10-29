#include "Player.hpp"
#include <cmath>

// Constructor
Player::Player(PlayerID playerId, Vector2 startPos, Color playerColor, PlayerControls playerControls)
    : position(startPos)
    , velocity({0.0f, 0.0f})
    , size({30.0f, 40.0f})
    , id(playerId)
    , color(playerColor)
    , isGrounded(false)
    , isAlive(true)
    , hasReachedExit(false)
    , controls(playerControls)
{
}

// Manejo de input
void Player::handleInput() {
    if (!isAlive) return;
    
    // Movimiento horizontal
    if (IsKeyDown(controls.left)) {
        velocity.x = -MOVE_SPEED;
    } else if (IsKeyDown(controls.right)) {
        velocity.x = MOVE_SPEED;
    } else {
        velocity.x = 0.0f;  // Parar inmediatamente (sin inercia en Hito 1)
    }
    
    // Salto (solo si está en el suelo)
    if (canJump()) {
        if (IsKeyPressed(controls.jump) || IsKeyPressed(controls.jump_alt)) {
            velocity.y = JUMP_FORCE;
            isGrounded = false;
        }
    }
}

// Aplicar gravedad
void Player::applyGravity(float gravity) {
    if (!isAlive) return;
    
    if (!isGrounded) {
        velocity.y += gravity;
    }
    
    // Limitar velocidad de caída
    clampVelocity();
}

// Actualizar posición
void Player::updatePosition(float deltaTime) {
    if (!isAlive) return;
    
    // Aplicar velocidad a posición
    position.x += velocity.x;
    position.y += velocity.y;
    
    // Restricción de límites horizontales de pantalla
    const float SCREEN_WIDTH = 1280.0f;
    if (position.x < 0) {
        position.x = 0;
        velocity.x = 0;
    }
    if (position.x + size.x > SCREEN_WIDTH) {
        position.x = SCREEN_WIDTH - size.x;
        velocity.x = 0;
    }
    
    // Detectar caída fuera de pantalla (muerte)
    const float SCREEN_HEIGHT = 730.0f;
    if (position.y > SCREEN_HEIGHT + 50.0f) {  // Si Y > 770
        isAlive = false;
    }
}

// Resetear jugador
void Player::reset(Vector2 startPos) {
    position = startPos;
    velocity = {0.0f, 0.0f};
    isGrounded = false;
    isAlive = true;
    hasReachedExit = false;
}

// Renderizar jugador
void Player::render() const {
    if (!isAlive) return;
    
    // Dibujar rectángulo del jugador
    DrawRectangleV(position, size, color);
    
    // Dibujar borde
    DrawRectangleLinesEx({position.x, position.y, size.x, size.y}, 2, BLACK);
    
    // Dibujar identificador de jugador
    const char* playerText = getPlayerName();
    int textWidth = MeasureText(playerText, 20);
    DrawText(playerText, 
             position.x + (size.x - textWidth) / 2, 
             position.y + (size.y - 20) / 2,
             20, 
             WHITE);
}

// Obtener rectángulo de colisión
Rectangle Player::getCollisionRect() const {
    return {position.x, position.y, size.x, size.y};
}

// Obtener nombre del jugador
const char* Player::getPlayerName() const {
    switch (id) {
        case PlayerID::PLAYER_1: return "P1";
        case PlayerID::PLAYER_2: return "P2";
        case PlayerID::PLAYER_3: return "P3";
        case PlayerID::PLAYER_4: return "P4";
        case PlayerID::PLAYER_5: return "P5";
        default: return "P?";
    }
}

// Verificar si puede saltar
bool Player::canJump() const {
    return isAlive && isGrounded;
}

// Limitar velocidad vertical
void Player::clampVelocity() {
    if (velocity.y > MAX_FALL_SPEED) {
        velocity.y = MAX_FALL_SPEED;
    }
}

// ============================================
// FUNCIONES AUXILIARES PARA CREAR JUGADORES
// ============================================

Player createPlayer1(Vector2 startPos) {
    PlayerControls controls;
    controls.left = KEY_A;
    controls.right = KEY_D;
    controls.jump = KEY_W;
    controls.jump_alt = KEY_SPACE;
    
    return Player(PlayerID::PLAYER_1, startPos, BLUE, controls);
}

Player createPlayer2(Vector2 startPos) {
    PlayerControls controls;
    controls.left = KEY_LEFT;
    controls.right = KEY_RIGHT;
    controls.jump = KEY_UP;
    controls.jump_alt = KEY_UP;  // Sin alternativa
    
    return Player(PlayerID::PLAYER_2, startPos, RED, controls);
}

Player createPlayer3(Vector2 startPos) {
    PlayerControls controls;
    controls.left = KEY_J;
    controls.right = KEY_L;
    controls.jump = KEY_I;
    controls.jump_alt = KEY_I;
    
    return Player(PlayerID::PLAYER_3, startPos, GREEN, controls);
}

Player createPlayer4(Vector2 startPos) {
    PlayerControls controls;
    controls.left = KEY_F;
    controls.right = KEY_H;
    controls.jump = KEY_T;
    controls.jump_alt = KEY_T;
    
    return Player(PlayerID::PLAYER_4, startPos, YELLOW, controls);
}

Player createPlayer5(Vector2 startPos) {
    PlayerControls controls;
    controls.left = KEY_LEFT;  // Estas teclas se cambiarían por gamepad
    controls.right = KEY_RIGHT;
    controls.jump = KEY_UP;
    controls.jump_alt = KEY_UP;
    
    return Player(PlayerID::PLAYER_5, startPos, PURPLE, controls);
}