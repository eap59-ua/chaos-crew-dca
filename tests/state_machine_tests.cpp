#define BOOST_TEST_MODULE StateMachineTests
#include <boost/test/included/unit_test.hpp>
#include <memory>
#include <string>
#include <stack>

// ============================================================================
// MÁQUINA DE ESTADOS DEL JUEGO (CASO 5)
// ============================================================================

// Estado base (interfaz)
class GameState {
public:
    virtual ~GameState() = default;
    virtual void onEnter() = 0;   // Llamado al entrar al estado
    virtual void onExit() = 0;    // Llamado al salir del estado
    virtual void update(float dt) = 0;
    virtual std::string getName() const = 0;
    
    bool isActive = false;
};

// Estados concretos del juego
class MenuState : public GameState {
public:
    void onEnter() override { isActive = true; enterCount++; }
    void onExit() override { isActive = false; exitCount++; }
    void update(float dt) override { updateCount++; }
    std::string getName() const override { return "Menu"; }
    
    int enterCount = 0;
    int exitCount = 0;
    int updateCount = 0;
};

class GamePlayState : public GameState {
public:
    void onEnter() override { isActive = true; enterCount++; }
    void onExit() override { isActive = false; exitCount++; }
    void update(float dt) override { updateCount++; }
    std::string getName() const override { return "GamePlay"; }
    
    int enterCount = 0;
    int exitCount = 0;
    int updateCount = 0;
};

class PauseState : public GameState {
public:
    void onEnter() override { isActive = true; enterCount++; }
    void onExit() override { isActive = false; exitCount++; }
    void update(float dt) override { updateCount++; }
    std::string getName() const override { return "Pause"; }
    
    int enterCount = 0;
    int exitCount = 0;
    int updateCount = 0;
};

// Máquina de estados (Stack-based FSM)
class StateMachine {
private:
    std::stack<std::shared_ptr<GameState>> states;
    
public:
    // Cambiar completamente de estado (pop + push)
    void changeState(std::shared_ptr<GameState> state) {
        if (!states.empty()) {
            states.top()->onExit();
            states.pop();
        }
        states.push(state);
        state->onEnter();
    }
    
    // Apilar nuevo estado (para pausar, por ejemplo)
    void pushState(std::shared_ptr<GameState> state) {
        if (!states.empty()) {
            states.top()->onExit();  // Pausar estado actual
        }
        states.push(state);
        state->onEnter();
    }
    
    // Desapilar estado (para reanudar)
    void popState() {
        if (!states.empty()) {
            states.top()->onExit();
            states.pop();
            
            if (!states.empty()) {
                states.top()->onEnter();  // Reanudar estado anterior
            }
        }
    }
    
    void update(float dt) {
        if (!states.empty()) {
            states.top()->update(dt);
        }
    }
    
    GameState* getCurrentState() {
        return states.empty() ? nullptr : states.top().get();
    }
    
    size_t getStateCount() const {
        return states.size();
    }
};

// ============================================================================
// TEST #6: Cambiar de estado (CASO 5)
// ============================================================================
// ¿Qué testea? Que se puede cambiar de un estado a otro
// ¿Por qué? El juego necesita ir de Menu → GamePlay → GameOver
// Teoría: Unit test, verifica transiciones de estado

BOOST_AUTO_TEST_CASE(test_change_state) {
    // ===== ARRANGE =====
    StateMachine sm;
    auto menuState = std::make_shared<MenuState>();
    auto gameState = std::make_shared<GamePlayState>();
    
    // Iniciar en menú
    sm.changeState(menuState);
    BOOST_REQUIRE(menuState->isActive);  // REQUIRE: crítico para el test
    
    // ===== ACT =====
    // Cambiar a estado de juego
    sm.changeState(gameState);
    
    // ===== ASSERT =====
    // Verificar que:
    // 1. El menú se desactivó
    BOOST_TEST(!menuState->isActive, "Menu state should be inactive");
    
    // 2. El juego se activó
    BOOST_TEST(gameState->isActive, "GamePlay state should be active");
    
    // 3. Se llamó a los callbacks correctos
    BOOST_TEST(menuState->exitCount == 1, "Menu onExit should be called once");
    BOOST_TEST(gameState->enterCount == 1, "GamePlay onEnter should be called once");
    
    // 4. El estado actual es GamePlay
    BOOST_TEST(sm.getCurrentState()->getName() == "GamePlay");
}

// ============================================================================
// TEST #7: Apilar estados (pausar juego) (CASO 5)
// ============================================================================
// ¿Qué testea? Que se puede pausar el juego apilando un estado
// ¿Por qué? Cuando pausas, el GamePlay se congela y aparece el menú de pausa
// Teoría: Unit test, verifica stack de estados

BOOST_AUTO_TEST_CASE(test_push_state) {
    // ===== ARRANGE =====
    StateMachine sm;
    auto gameState = std::make_shared<GamePlayState>();
    auto pauseState = std::make_shared<PauseState>();
    
    // Iniciar jugando
    sm.changeState(gameState);
    
    // ===== ACT =====
    // Pausar el juego (apilar estado de pausa)
    sm.pushState(pauseState);
    
    // ===== ASSERT =====
    // Verificar que hay 2 estados en la pila
    BOOST_TEST(sm.getStateCount() == 2, "Should have 2 states in stack");
    
    // El GamePlay se pausó (está inactivo pero sigue en la pila)
    BOOST_TEST(!gameState->isActive, "GamePlay should be paused (inactive)");
    
    // El estado de pausa está activo
    BOOST_TEST(pauseState->isActive, "Pause state should be active");
    
    // El estado actual es Pause
    BOOST_TEST(sm.getCurrentState()->getName() == "Pause");
    
    // EXTRA: Verificar que solo el estado activo recibe updates
    sm.update(0.016f);  // Un frame a 60 FPS
    BOOST_TEST(pauseState->updateCount == 1, "Active state should be updated");
    BOOST_TEST(gameState->updateCount == 0, "Inactive state should NOT be updated");
}

// ============================================================================
// NOTAS TEÓRICAS - MÁQUINAS DE ESTADOS
// ============================================================================
/*
¿Qué es una Finite State Machine (FSM)?
- Sistema que está en exactamente UN estado a la vez
- Puede transicionar entre estados según reglas
- Ejemplo en juegos: Menu → Playing → Paused → GameOver

Tipos de FSM:
1. Simple FSM: Solo un estado activo (changeState)
2. Stack-based FSM: Pila de estados (pushState/popState)

¿Por qué Stack-based?
- Permite "pausar" estados (push PauseState)
- Al reanudar, vuelves al estado anterior (pop)
- Útil para: Pause, Diálogos, Inventarios

Ciclo de vida de un estado:
1. onEnter() → Inicializar (cargar recursos, setup)
2. update(dt) → Actualizar cada frame
3. onExit() → Limpiar (liberar recursos)

Ejemplo de uso en el juego:
void Game::handlePauseButton() {
    if (input.isPausePressed()) {
        stateMachine.pushState(std::make_shared<PauseState>());
    }
}

void Game::handleResumeButton() {
    stateMachine.popState();  // Vuelve a GamePlayState
}

¿Por qué testear esto?
✅ Detecta bugs en transiciones (ej: olvidar llamar onExit)
✅ Verifica que el stack funciona correctamente
✅ Asegura que solo el estado activo recibe updates
✅ Previene memory leaks (estados que no se limpian)

Test #6: Transiciones simples (Menu → GamePlay)
Test #7: Stack de estados (GamePlay → Pause → GamePlay)
*/