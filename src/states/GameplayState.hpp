#ifndef GAMEPLAYSTATE_HPP
#define GAMEPLAYSTATE_HPP

#include "../core/GameState.hpp"
#include "../core/StateMachine.hpp"
#include <vector>
#include <raylib.h>
#include "../entt/entt.hpp"
// IMPORTANTE: Incluir el componente de animaciones
#include "../components/PlayerAnimations.hpp"

class GameOverState;

class GameplayState : public GameState {
public:
    explicit GameplayState(std::string mapPath);
    ~GameplayState();

    void init() override;
    void handleInput() override;
    void update(float deltaTime) override;
    void render() override;
    void pause() override {}
    void resume() override {}

    std::string getMapPath() const { return selectedMapPath; }

private:
    entt::registry registry;
    
    // Estructuras para guardar las 3 animaciones de cada jugador
    PlayerAnimations p1Anims;
    PlayerAnimations p2Anims;

    // --- RECURSOS GRÁFICOS ---
    Texture2D backgroundTexture;
    Texture2D terrainTexture;
    Texture2D doorTexture;

    // --- RECURSOS DE AUDIO ---
    Music bgMusic;
    Sound jumpSfx;
    Sound winSfx;
    Sound loseSfx;
    
    // --- VARIABLES DE ESTADO ---
    bool levelCompleted;
    bool isGameOver;
    bool isExitVisible;
    bool isExitMoved;

    // --- NUEVO: Lógica de espera para terminar nivel ---
    float finishTimer;      // Cronómetro
    bool isFinishing;       // ¿Estamos esperando para salir?
    bool won;               // ¿Ganamos o perdimos?
    
    static constexpr int SCREEN_WIDTH = 1280;
    static constexpr int SCREEN_HEIGHT = 720;

    std::string selectedMapPath;
    
    void setupPlayers();
    void setupPlatforms();
    void checkVictoryCondition();
    void checkDefeatCondition();
    void handleTrollMechanic();
};

#endif // GAMEPLAYSTATE_HPP