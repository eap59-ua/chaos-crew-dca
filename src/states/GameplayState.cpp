#include "GameplayState.hpp"
#include "GameOverState.hpp"  // ✅ Incluir en .cpp

GameplayState::GameplayState() 
    : exitZone(SCREEN_WIDTH - 100, SCREEN_HEIGHT - 150, 80, 100)
    , levelCompleted(false)
    , isGameOver(false)
    , isExitVisible(true)
    , isExitMoved(false)
{
}

void GameplayState::init() {
    // Limpiar estado anterior
    players.clear();
    platforms.clear();
    
    // Configurar jugadores y nivel
    setupPlayers();
    setupPlatforms();
    
    // Resetear flags
    levelCompleted = false;
    isGameOver = false;
    isExitVisible = true;
    isExitMoved = false;
    exitZone.reset();
    
    // Posicionar zona de salida al final del nivel
    exitZone.rect.x = SCREEN_WIDTH - 100;
    exitZone.rect.y = SCREEN_HEIGHT - 150;
}

void GameplayState::setupPlayers() {
    // Crear jugador 1 (Azul - WASD)
    Player p1 = createPlayer1({100, SCREEN_HEIGHT - 200});
    players.push_back(p1);
    
    // Crear jugador 2 (Rojo - Flechas)
    Player p2 = createPlayer2({150, SCREEN_HEIGHT - 200});
    players.push_back(p2);
    
    // TODO: Para Hito 2, añadir soporte para 3-5 jugadores
    // Player p3 = createPlayer3({200, SCREEN_HEIGHT - 200});
    // players.push_back(p3);
}

void GameplayState::setupPlatforms() {
    // Suelo principal
    platforms.push_back(Platform::createGround(SCREEN_WIDTH));
    
    // Plataformas intermedias
    platforms.push_back(Platform::createNormalPlatform(200, SCREEN_HEIGHT - 200, 200));
    platforms.push_back(Platform::createNormalPlatform(500, SCREEN_HEIGHT - 300, 200));
    platforms.push_back(Platform::createNormalPlatform(800, SCREEN_HEIGHT - 400, 200));
    platforms.push_back(Platform::createNormalPlatform(1000, SCREEN_HEIGHT - 250, 150));
    
    // TODO: Para Hito 2, añadir:
    // - Plataformas móviles
    // - Plataformas que desaparecen
    // - Trampas dinámicas
}

void GameplayState::handleInput() {
    // Reinicio manual con ENTER (útil para debugging)
    if (IsKeyPressed(KEY_ENTER)) {
        init();
        return;
    }
    
    // Procesar input de cada jugador
    for (auto& player : players) {
        player.handleInput();
    }
}

void GameplayState::update(float deltaTime) {
    // Actualizar físicas de todos los jugadores
    physicsEngine.updateAllPlayers(players, deltaTime);
    
    // Procesar colisiones con plataformas
    physicsEngine.processAllCollisions(players, platforms);
    
    // Verificar condiciones de victoria/derrota
    checkDefeatCondition();
    if (isGameOver) {
        state_machine->add_state(std::make_unique<GameOverState>(false), true);
        return;
    }
    
    // Mecánica del nivel troll (Hito 1)
    handleTrollMechanic();
    
    // Verificar victoria
    checkVictoryCondition();
    if (levelCompleted) {
        state_machine->add_state(std::make_unique<GameOverState>(true), true);
        return;
    }
}

void GameplayState::checkVictoryCondition() {
    if (players.size() < 2) return;  // Necesitamos al menos 2 jugadores
    
    // Verificar si cada jugador está en la zona de salida
    exitZone.player1Inside = physicsEngine.checkExitCollision(players[0], exitZone);
    exitZone.player2Inside = physicsEngine.checkExitCollision(players[1], exitZone);
    
    // Victoria: AMBOS jugadores deben estar en la salida Y el nivel troll debe estar completo
    if (isExitMoved && exitZone.bothPlayersInside()) {
        levelCompleted = true;
    }
}

void GameplayState::checkDefeatCondition() {
    // Derrota: si CUALQUIER jugador ha muerto
    for (const auto& player : players) {
        if (!player.isAlive) {
            isGameOver = true;
            return;
        }
    }
}

void GameplayState::handleTrollMechanic() {
    // Nivel troll del Hito 1: la salida se mueve cuando ambos jugadores se acercan
    if (isExitMoved || players.size() < 2) return;
    
    // Verificar si ambos jugadores están cerca de la salida
    bool player1Near = players[0].position.x > (exitZone.rect.x - 50);
    bool player2Near = players[1].position.x > (exitZone.rect.x - 50);
    
    if (player1Near && player2Near) {
        // ¡Sorpresa! La salida se mueve al principio del nivel
        exitZone.rect.x = 20;
        exitZone.rect.y = SCREEN_HEIGHT - 150;
        
        isExitMoved = true;
        isExitVisible = true;
        
        // Resetear los flags de jugadores dentro
        exitZone.reset();
    }
}

void GameplayState::render() {
    BeginDrawing();
    ClearBackground(RAYWHITE);
    
    // Renderizar plataformas
    for (const auto& platform : platforms) {
        platform.render();
    }
    
    // Renderizar zona de salida (si es visible)
    if (isExitVisible) {
        exitZone.render();
    }
    
    // Renderizar jugadores
    for (const auto& player : players) {
        player.render();
    }
    
    // HUD superior
    DrawRectangle(0, 0, SCREEN_WIDTH, 100, Fade(BLACK, 0.7f));
    DrawText("CHAOS CREW - Hito 1 Alpha", 20, 10, 30, YELLOW);
    DrawText("P1: A/D move, W jump | P2: Arrows move, UP jump", 20, 45, 20, GRAY);
    DrawText("COOPERATIVE: Both must reach EXIT!", 20, 70, 18, GREEN);
    
    // Indicador de FPS (útil para debugging)
    DrawText(TextFormat("FPS: %d", GetFPS()), SCREEN_WIDTH - 100, 10, 20, LIME);
    
    // Indicador si la salida se ha movido (nivel troll)
    if (isExitMoved) {
        DrawText("The exit moved! Go back!", SCREEN_WIDTH/2 - 150, 120, 25, RED);
    }
    
    EndDrawing();
}