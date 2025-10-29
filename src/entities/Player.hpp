#ifndef PLAYER_HPP
#define PLAYER_HPP

#include <raylib.h>
#include <string>

enum class PlayerID {
    PLAYER_1,
    PLAYER_2,
    PLAYER_3,  // Para futuro multijugador
    PLAYER_4,
    PLAYER_5
};

struct PlayerControls {
    KeyboardKey left;
    KeyboardKey right;
    KeyboardKey jump;
    KeyboardKey jump_alt;  // Alternativa (ej: SPACE para P1)
};

class Player {
public:
    // Propiedades físicas
    Vector2 position;
    Vector2 velocity;
    Vector2 size;
    
    // Estado del jugador
    PlayerID id;
    Color color;
    bool isGrounded;
    bool isAlive;
    bool hasReachedExit;
    
    // Controles
    PlayerControls controls;
    
    // Constantes de gameplay
    static constexpr float MOVE_SPEED = 4.5f;
    static constexpr float JUMP_FORCE = -16.0f;
    static constexpr float MAX_FALL_SPEED = 20.0f;
    
    // Constructor
    Player(PlayerID playerId, Vector2 startPos, Color playerColor, PlayerControls playerControls);
    
    // Métodos principales
    void handleInput();
    void applyGravity(float gravity);
    void updatePosition(float deltaTime);
    void reset(Vector2 startPos);
    void render() const;
    
    // Getters útiles
    Rectangle getCollisionRect() const;
    const char* getPlayerName() const;
    bool canJump() const;
    
private:
    void clampVelocity();
};

// Funciones auxiliares para crear jugadores predefinidos
Player createPlayer1(Vector2 startPos);
Player createPlayer2(Vector2 startPos);
Player createPlayer3(Vector2 startPos);
Player createPlayer4(Vector2 startPos);
Player createPlayer5(Vector2 startPos);

#endif // PLAYER_HPP