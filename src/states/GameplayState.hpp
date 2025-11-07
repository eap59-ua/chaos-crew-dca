#ifndef GAMEPLAYSTATE_HPP
#define GAMEPLAYSTATE_HPP

#include "../core/GameState.hpp"
#include "../core/StateMachine.hpp"
// #include "../entities/Player.hpp"
// #include "../entities/Platform.hpp"
// #include "../physics/Physics.hpp"
#include <vector>
#include <raylib.h>
#include "../entt/entt.hpp"


// Forward declaration para evitar dependencia circular
class GameOverState;

class GameplayState : public GameState {
public:
    GameplayState();
    ~GameplayState() override = default;

    void init() override;
    void handleInput() override;
    void update(float deltaTime) override;
    void render() override;
    void pause() override {}
    void resume() override {}

private:
    entt::registry registry;
    // Entidades del juego
    //std::vector<Player> players;
    //std::vector<Platform> platforms;
    
    // Motor de físicas
    // PhysicsEngine physicsEngine;
    
    // Estado del nivel
    bool levelCompleted;
    bool isGameOver;
    
    // Nivel troll (Hito 1) - mantener la funcionalidad original
    bool isExitVisible;
    bool isExitMoved;
    
    // Constantes de pantalla
    static constexpr int SCREEN_WIDTH = 1280;
    static constexpr int SCREEN_HEIGHT = 720;
    
    // Métodos auxiliares
    void setupPlayers();
    void setupPlatforms();
    void checkVictoryCondition();
    void checkDefeatCondition();
    void handleTrollMechanic();  // Para el nivel troll del Hito 1
};

#endif // GAMEPLAYSTATE_HPP