#include "GameplayState.hpp"
#include "GameOverState.hpp"
#include "PauseState.hpp"
#include "../entities/PlayerFactory.hpp"
#include "../entities/PlatformFactory.hpp"
#include "../entities/DoorFactory.hpp"
#include "../systems/RenderSystem.hpp"
#include "../systems/MovimentSystem.hpp"
#include "../systems/InputSystem.hpp"
#include "../systems/CollisionSystem.hpp"
#include "../systems/WinSystem.hpp"
#include "../systems/TrollSystem.hpp"
#include "../systems/LoadMapSystem.hpp"
#include "../systems/PatronSystem.hpp"

// Incluimos el Sistema de Animación
#include "../systems/AnimationSystem.hpp"

#include <filesystem>
GameplayState::GameplayState(std::string mapPath) 

    : levelCompleted(false)
    , isGameOver(false)
    , isExitVisible(true)
    , isExitMoved(false)
    // --- INICIALIZACIÓN DE VARIABLES DE TIMER ---
    , finishTimer(0.0f)
    , isFinishing(false)
    , won(false)
    , selectedMapPath(std::move(mapPath)) //move hace que no se copie el string, sino que se transfiera su propiedad
{
    // Inicializamos las estructuras a 0 para seguridad
    p1Anims = { {0}, {0}, {0} };
    p2Anims = { {0}, {0}, {0} };
    backgroundTexture = {0};
    terrainTexture = {0};
    doorTexture = {0};
    bgMusic = {0};
    jumpSfx = {0};
    winSfx = {0};
    loseSfx = {0};
}

GameplayState::~GameplayState() {
    // --- LIMPIEZA DE RECURSOS ---
    
    // Jugador 1
    if (p1Anims.idle.id != 0) UnloadTexture(p1Anims.idle);
    if (p1Anims.run.id != 0)  UnloadTexture(p1Anims.run);
    if (p1Anims.jump.id != 0) UnloadTexture(p1Anims.jump);
    
    // Jugador 2
    if (p2Anims.idle.id != 0) UnloadTexture(p2Anims.idle);
    if (p2Anims.run.id != 0)  UnloadTexture(p2Anims.run);
    if (p2Anims.jump.id != 0) UnloadTexture(p2Anims.jump);

    // Entorno
    if (backgroundTexture.id != 0) UnloadTexture(backgroundTexture);
    if (terrainTexture.id != 0) UnloadTexture(terrainTexture);
    if (doorTexture.id != 0) UnloadTexture(doorTexture);

    // Audio
    if (bgMusic.stream.buffer != nullptr) UnloadMusicStream(bgMusic);
    if (jumpSfx.stream.buffer != nullptr) UnloadSound(jumpSfx);
    if (winSfx.stream.buffer != nullptr)  UnloadSound(winSfx);
    if (loseSfx.stream.buffer != nullptr) UnloadSound(loseSfx);
}

void GameplayState::init() {
    // 1. Limpiar registro anterior
    registry.clear();
    
    // 2. CARGAR RECURSOS
    
    // --- JUGADOR 1 (Virtual Guy) ---
    p1Anims.idle = LoadTexture("assets/images/VirtualGuy/Idle (32x32).png");
    p1Anims.run  = LoadTexture("assets/images/VirtualGuy/Run (32x32).png");
    p1Anims.jump = LoadTexture("assets/images/VirtualGuy/Jump (32x32).png");

    // --- JUGADOR 2 (Pink Man) ---
    p2Anims.idle = LoadTexture("assets/images/PinkMan/Idle (32x32).png");
    p2Anims.run  = LoadTexture("assets/images/PinkMan/Run (32x32).png");
    p2Anims.jump = LoadTexture("assets/images/PinkMan/Jump (32x32).png");
    
    // --- ENTORNO ---
    backgroundTexture = LoadTexture("assets/images/Background/Brown.png"); 
    terrainTexture = LoadTexture("assets/images/Terrain/Terrain (32x32).png");
    doorTexture = LoadTexture("assets/images/Door.png"); // Asegúrate que la ruta sea correcta

    // --- AUDIO ---
    bgMusic = LoadMusicStream("assets/sounds/Theme.wav");
    jumpSfx = LoadSound("assets/sounds/Jump.wav");
    winSfx = LoadSound("assets/sounds/Win.wav");
    loseSfx = LoadSound("assets/sounds/Lose.wav");

    SetMusicVolume(bgMusic, 0.5f); 
    PlayMusicStream(bgMusic);

    // 3. Configurar escena
    setupPlayers();
    loadTiledMap(selectedMapPath, registry);
    
    // Resetear flags
    levelCompleted = false;
    isGameOver = false;
    isExitVisible = true;
    isExitMoved = false;
    
    // Resetear timer
    isFinishing = false;
    finishTimer = 0.0f;
}

void GameplayState::setupPlayers() {
    // P1: Animaciones Virtual Guy
    createPlayer(registry, 100, SCREEN_HEIGHT - 200, p1Anims, KEY_LEFT, KEY_RIGHT, KEY_UP);
    // P2: Animaciones Pink Man
    createPlayer(registry, 150, SCREEN_HEIGHT - 200, p2Anims, KEY_A, KEY_D, KEY_W);
}

void GameplayState::setupPlatforms() {
    // Vacío (se carga desde XML)
}

void GameplayState::handleInput() {
    // Si estamos terminando (esperando sonido), bloqueamos el input
    if (isFinishing) return;

    if (IsKeyPressed(KEY_ENTER)) {
        init();
        return;
    }
    if (IsKeyPressed(KEY_ESCAPE)) {
        state_machine->add_state(std::make_unique<PauseState>(selectedMapPath), false);
        return;
    }

    InputSystem(registry);
}

void GameplayState::update(float deltaTime) {
    // Siempre actualizamos la música para que no se corte el loop
    UpdateMusicStream(bgMusic);

    // --- LÓGICA DE ESPERA PARA SONIDOS ---
    if (isFinishing) {
        finishTimer += deltaTime;
        
        // Esperamos 2.0 segundos (ajusta este valor si el sonido es más largo/corto)
        if (finishTimer >= 2.0f) {
            state_machine->add_state(std::make_unique<GameOverState>(won, selectedMapPath), true);
        }
        // IMPORTANTE: Return aquí para NO procesar movimiento ni colisiones mientras esperamos
        return; 
    }
    // -------------------------------------

    // 1. Movimiento y Físicas
    MovementSystem(registry, deltaTime, SCREEN_WIDTH, SCREEN_HEIGHT, jumpSfx);
    CollisionSystem(registry);
    
    // 2. Animación
    AnimationSystem(registry, deltaTime);

    // 3. Lógica del Nivel
    TrapSystem(registry, deltaTime);
    PatronSystem(registry, deltaTime);
    
    // 4. Condiciones de Fin de Juego
    if (CheckDefeat(registry)) {
        PlaySound(loseSfx); // Reproducir sonido
        
        // Iniciar cuenta atrás en lugar de salir ya
        isFinishing = true;
        won = false;
        finishTimer = 0.0f;
        state_machine->add_state(std::make_unique<GameOverState>(false, selectedMapPath), true);
        return;
    }
    
    if (CheckVictory(registry)) {
        PlaySound(winSfx); // Reproducir sonido
        
        isFinishing = true;
        won = true;
        finishTimer = 0.0f;
        state_machine->add_state(std::make_unique<GameOverState>(true, selectedMapPath), true);
        return;
    }
}

// Métodos vacíos mantenidos por compatibilidad
void GameplayState::checkVictoryCondition() {}
void GameplayState::checkDefeatCondition() {}
void GameplayState::handleTrollMechanic() {}

void GameplayState::render() {

    BeginDrawing();
    
    // Fondo
    if (backgroundTexture.id != 0) {
        DrawTexturePro(backgroundTexture,
                       Rectangle{0, 0, (float)backgroundTexture.width, (float)backgroundTexture.height},
                       Rectangle{0, 0, (float)SCREEN_WIDTH, (float)SCREEN_HEIGHT},
                       Vector2{0, 0}, 0.0f, WHITE);
    } else {
        ClearBackground(RAYWHITE);
    }
    
    // Escena (Plataformas, Puertas, Jugadores)
    renderScene(registry, terrainTexture, doorTexture);
    
    // HUD
    DrawRectangle(0, 0, SCREEN_WIDTH, 100, Fade(BLACK, 0.7f));
    DrawText("CHAOS CREW - Hito 1 Alpha", 20, 10, 30, YELLOW);
    DrawText("P1: Arrows | P2: WASD", 20, 45, 20, GRAY);
    DrawText("COOPERATIVE: Both must reach EXIT!", 20, 70, 18, GREEN);
    
    DrawText(TextFormat("FPS: %d", GetFPS()), SCREEN_WIDTH - 100, 10, 20, LIME);
    
    if (isExitMoved) {
        DrawText("The exit moved! Go back!", SCREEN_WIDTH/2 - 150, 120, 25, RED);
    }

    // Mensaje opcional mientras se espera el sonido final
    if (isFinishing) {
        const char* msg = won ? "VICTORY!" : "DEFEAT!";
        Color color = won ? GREEN : RED;
        DrawText(msg, SCREEN_WIDTH/2 - MeasureText(msg, 60)/2, SCREEN_HEIGHT/2, 60, color);
    }
    
    EndDrawing();
}