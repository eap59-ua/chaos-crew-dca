#pragma once

class StateMachine;

/**
 * Base abstracta para todos los estados del juego.
 * Cada estado (Gameplay, GameOver, Menú, etc.) hereda de aquí.
 */
class GameState {
public:
    GameState();
    virtual ~GameState() = default;

    // Ciclo de vida del estado
    virtual void init() = 0;                 // Se llama al entrar por primera vez
    virtual void handleInput() = 0;          // Lectura de input
    virtual void update(float deltaTime) = 0;// Lógica (usa deltaTime)
    virtual void render() = 0;               // Dibujo

    // Cuando el estado queda tapado por otro (pause/resume opcional)
    virtual void pause() = 0;
    virtual void resume() = 0;

    // La StateMachine se inyecta automáticamente al hacer add_state()
    void setStateMachine(StateMachine* sm) { state_machine = sm; }

protected:
    StateMachine* state_machine;  // no-ownership
};
