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

#include "../core/ResourceManager.h"
#include "../locale/Locale.hpp"
#include "../utils/Logger.hpp"

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
    // Las texturas son gestionadas automáticamente por ResourceManager
    // Solo limpiamos el audio aquí

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
    p1Anims.idle = ResourceManager::GetInstance().GetTexture("assets/images/VirtualGuy/Idle (32x32).png");
    p1Anims.run  = ResourceManager::GetInstance().GetTexture("assets/images/VirtualGuy/Run (32x32).png");
    p1Anims.jump = ResourceManager::GetInstance().GetTexture("assets/images/VirtualGuy/Jump (32x32).png");

    // --- JUGADOR 2 (Pink Man) ---
    p2Anims.idle = ResourceManager::GetInstance().GetTexture("assets/images/PinkMan/Idle (32x32).png");
    p2Anims.run  = ResourceManager::GetInstance().GetTexture("assets/images/PinkMan/Run (32x32).png");
    p2Anims.jump = ResourceManager::GetInstance().GetTexture("assets/images/PinkMan/Jump (32x32).png");
    
    // --- ENTORNO ---
    backgroundTexture = ResourceManager::GetInstance().GetTexture("assets/images/Background/Brown.png");
    terrainTexture = ResourceManager::GetInstance().GetTexture("assets/images/Terrain/Terrain (32x32).png");
    doorTexture = ResourceManager::GetInstance().GetTexture("assets/images/Door.png"); // Asegúrate que la ruta sea correcta

    // --- TRAMPAS ---
    trapSpikeTexture = ResourceManager::GetInstance().GetTexture("assets/images/Spikes/Idle.png");
    trapWheelTexture = ResourceManager::GetInstance().GetTexture("assets/images/Spike Head/Blink (54x52).png");

    SetTextureWrap(trapSpikeTexture, TEXTURE_WRAP_CLAMP); // Evitar repetición
    SetTextureFilter(trapSpikeTexture, TEXTURE_FILTER_POINT); // Pixel art nítido
    SetTextureFilter(trapWheelTexture, TEXTURE_FILTER_POINT); // Pixel art nítido

    // --- AUDIO ---
    bgMusic = LoadMusicStream("assets/sounds/Theme.wav");
    jumpSfx = LoadSound("assets/sounds/Jump.wav");
    winSfx = LoadSound("assets/sounds/Win.wav");
    loseSfx = LoadSound("assets/sounds/Lose.wav");

    SetMusicVolume(bgMusic, 0.5f); 
    PlayMusicStream(bgMusic);

    // 3. Configurar escena
    setupPlayers();
    loadTiledMap(selectedMapPath, registry, trapSpikeTexture, trapWheelTexture);
    
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
    LOG_INFO("[GameplayState] ========== PLAYER SETUP START ==========");

    // P1: Keyboard player 1 (Arrows) - Virtual Guy
    createPlayer(registry, 100, SCREEN_HEIGHT - 200, p1Anims, KEY_LEFT, KEY_RIGHT, KEY_UP);
    LOG_INFO("[GameplayState] Player 1 created with keyboard (Arrows)");

    // P2: Keyboard player 2 (WASD) - Pink Man
    createPlayer(registry, 150, SCREEN_HEIGHT - 200, p2Anims, KEY_A, KEY_D, KEY_W);
    LOG_INFO("[GameplayState] Player 2 created with keyboard (WASD)");

    // FORCE polling before detection
    LOG_INFO("[GameplayState] Checking for gamepads...");
    PollInputEvents();

    // Detect and create gamepad players (P3, P4, P5)
    int playerCount = 2; // Already have 2 keyboard players
    const int MAX_PLAYERS = 5;

    for (int gamepadIndex = 0; gamepadIndex < 4 && playerCount < MAX_PLAYERS; gamepadIndex++) {
        bool available = IsGamepadAvailable(gamepadIndex);
        LOG_INFO("[GameplayState] Gamepad {} availability: {}", gamepadIndex, available ? "YES" : "NO");

        if (available) {
            const char* gamepadName = GetGamepadName(gamepadIndex);
            LOG_INFO("[GameplayState] Gamepad {} detected: {}", gamepadIndex, gamepadName);

            // Alternate animations between Virtual Guy and Pink Man
            PlayerAnimations& anims = (playerCount % 2 == 0) ? p1Anims : p2Anims;

            // Create gamepad player with offset position
            float xOffset = 50.0f * (playerCount - 1);
            createPlayerWithGamepad(registry, 100 + xOffset, SCREEN_HEIGHT - 200, anims, gamepadIndex);

            LOG_INFO("[GameplayState] Player {} created with Gamepad {}", playerCount + 1, gamepadIndex);
            playerCount++;
        }
    }

    if (playerCount == 2) {
        LOG_INFO("[GameplayState] No gamepads detected, using keyboard only");
    }
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

    // IMPORTANT: Poll for gamepad events every frame for hot-plugging support
    PollInputEvents();

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
    DrawText(_("CHAOS CREW - Hito 1 Alpha"), 20, 10, 30, YELLOW);
    DrawText(_("P1: Arrows | P2: WASD"), 20, 45, 20, GRAY);
    DrawText(_("COOPERATIVE: Both must reach EXIT!"), 20, 70, 18, GREEN);

    DrawText(TextFormat("FPS: %d", GetFPS()), SCREEN_WIDTH - 100, 10, 20, LIME);

    if (isExitMoved) {
        DrawText(_("The exit moved! Go back!"), SCREEN_WIDTH/2 - 150, 120, 25, RED);
    }

    // Mensaje opcional mientras se espera el sonido final
    if (isFinishing) {
        const char* msg = won ? _("VICTORY!") : _("DEFEAT!");
        Color color = won ? GREEN : RED;
        DrawText(msg, SCREEN_WIDTH/2 - MeasureText(msg, 60)/2, SCREEN_HEIGHT/2, 60, color);
    }
    
    EndDrawing();
}