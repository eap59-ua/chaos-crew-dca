#include "core/StateMachine.hpp"
#include "core/ResourceManager.h"
#include "states/GameplayState.hpp"
#include "states/MainMenuState.hpp"
#include "locale/Locale.hpp"
#include "utils/Logger.hpp"
#include "debug/DebugUI.hpp"
#include "systems/PhysicsConfig.hpp"
#include <raylib.h>
#include <cstdlib>
#include <cstring>

int main() {
    // ============================================================================
    // INICIALIZACIÓN DE LOGGER
    // ============================================================================
    Logger::GetInstance().Init("chaos-crew.log");
    LOG_INFO("=== Chaos Crew Starting ===");
    LOG_INFO("Version: 0.2.0");

    // ============================================================================
    // INICIALIZACIÓN DE INTERNACIONALIZACIÓN (i18n)
    // ============================================================================

    // Detectar idioma del sistema automáticamente
    std::string defaultLang = "es_ES";  // Español por defecto

    const char* sysLang = std::getenv("LANG");
    if (sysLang != nullptr) {
        if (std::strstr(sysLang, "en_") != nullptr) {
            defaultLang = "en_US";
        }
    }

    // Inicializar sistema de localización
    Locale::GetInstance().Init(defaultLang);
    LOG_INFO("i18n initialized with language: {}", defaultLang);

    // ============================================================================
    // CONFIGURACIÓN DE VENTANA Y AUDIO
    // ============================================================================

    const int SCREEN_WIDTH = 1280;
    const int SCREEN_HEIGHT = 720;

    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Chaos Crew");
    SetExitKey(KEY_NULL); //quitamos que con esc se salga del juego, para añadirlo en la pantalla de pausa.

    // ============================================================================
    // GAMEPAD DETECTION DEBUG
    // ============================================================================
    LOG_INFO("========== GAMEPAD DETECTION CHECK ==========");
    LOG_INFO("Checking for connected gamepads after InitWindow...");

    // Force poll inputs immediately after window creation
    PollInputEvents();

    for (int i = 0; i < 4; i++) {
        bool available = IsGamepadAvailable(i);
        if (available) {
            const char* name = GetGamepadName(i);
            LOG_INFO("Gamepad {} DETECTED: {}", i, name);
        } else {
            LOG_INFO("Gamepad {} NOT DETECTED", i);
        }
    }
    LOG_INFO("=============================================");

    InitAudioDevice();
    SetMasterVolume(1.0f);
    ResourceManager::GetInstance().PlayGlobalMusic("assets/sounds/Theme.wav");

    SetTargetFPS(60);

    // ============================================================================
    // INICIALIZACIÓN DE DEBUG UI
    // ============================================================================
    DebugUI::GetInstance().Init();

    // Registrar parámetros de física para tweaking en tiempo real
    auto& physics = PhysicsConfig::Instance();
    DebugUI::GetInstance().RegisterFloat("Horizontal Speed", &physics.H_SPEED, 50.0f, 500.0f);
    DebugUI::GetInstance().RegisterFloat("Air Control", &physics.AIR_CONTROL, 0.0f, 1.0f);
    DebugUI::GetInstance().RegisterFloat("Gravity", &physics.GRAVITY, 100.0f, 2000.0f);
    DebugUI::GetInstance().RegisterFloat("Jump Force", &physics.JUMP_FORCE, -1000.0f, -100.0f);
    DebugUI::GetInstance().RegisterFloat("Max Fall Speed", &physics.MAX_FALL_SPEED, 200.0f, 2000.0f);

    LOG_INFO("Debug UI initialized and physics parameters registered");
    
    // Crear máquina de estados
    StateMachine stateMachine;
    float deltaTime = 0.0f;
    
    // Cargar estado inicial (MainMenu)
    stateMachine.add_state(std::make_unique<MainMenuState>(), false);
    stateMachine.handle_state_changes(deltaTime);
    
    // Bucle principal del juego
    while (!stateMachine.is_game_ending() && !WindowShouldClose()) {
        deltaTime = GetFrameTime();
        ResourceManager::GetInstance().UpdateGlobalMusic();

        // ============================================================================
        // TECLAS DE DEBUG Y TESTING
        // ============================================================================

        // F1: Toggle Debug UI
        if (IsKeyPressed(KEY_F1)) {
            DebugUI::GetInstance().Toggle();
            LOG_DEBUG("Debug UI toggled: {}", DebugUI::GetInstance().IsEnabled() ? "ON" : "OFF");
        }

        // L: Cambio de idioma (para testing)
        if (IsKeyPressed(KEY_L)) {
            std::string currentLang = Locale::GetInstance().GetCurrentLanguage();
            std::string newLang = (currentLang == "es_ES") ? "en_US" : "es_ES";
            Locale::GetInstance().SetLanguage(newLang);
            LOG_INFO("Language changed: {} -> {}", currentLang, newLang);
        }

        // Manejar cambios de estado
        stateMachine.handle_state_changes(deltaTime);

        // Obtener estado actual
        auto& currentState = stateMachine.getCurrentState();

        // Actualizar estado actual
        currentState->handleInput();
        currentState->update(deltaTime);

        // Iniciar frame de ImGui antes de renderizar
        DebugUI::GetInstance().NewFrame();

        // Renderizar estado actual
        currentState->render();

        // Renderizar Debug UI al final (overlay sobre el juego)
        DebugUI::GetInstance().Render();
    }
    
    LOG_INFO("Game loop ended");

    CloseAudioDevice();

    // Cleanup recursos
    ResourceManager::GetInstance().UnloadAll();
    LOG_INFO("Resources unloaded");

    // Shutdown Debug UI
    DebugUI::GetInstance().Shutdown();

    CloseWindow();
    LOG_INFO("=== Chaos Crew Shutdown ===");

    return 0;
}