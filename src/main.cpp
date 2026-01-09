#include "core/StateMachine.hpp"
#include "core/ResourceManager.h"
#include "states/GameplayState.hpp"
#include "states/MainMenuState.hpp"
#include "locale/Locale.hpp"
#include <raylib.h>
#include <cstdlib>
#include <cstring>

int main() {
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

    // ============================================================================
    // CONFIGURACIÓN DE VENTANA Y AUDIO
    // ============================================================================

    const int SCREEN_WIDTH = 1280;
    const int SCREEN_HEIGHT = 720;

    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Chaos Crew");
    SetExitKey(KEY_NULL); //quitamos que con esc se salga del juego, para añadirlo en la pantalla de pausa.

    InitAudioDevice();
    SetMasterVolume(0.5f);

    SetTargetFPS(60);
    
    // Crear máquina de estados
    StateMachine stateMachine;
    float deltaTime = 0.0f;
    
    // Cargar estado inicial (MainMenu)
    stateMachine.add_state(std::make_unique<MainMenuState>(), false);
    stateMachine.handle_state_changes(deltaTime);
    
    // Bucle principal del juego
    while (!stateMachine.is_game_ending() && !WindowShouldClose()) {
        deltaTime = GetFrameTime();

        // ============================================================================
        // CAMBIO DE IDIOMA CON TECLA L (para testing)
        // ============================================================================
        if (IsKeyPressed(KEY_L)) {
            std::string currentLang = Locale::GetInstance().GetCurrentLanguage();
            std::string newLang = (currentLang == "es_ES") ? "en_US" : "es_ES";
            Locale::GetInstance().SetLanguage(newLang);
        }

        // Manejar cambios de estado
        stateMachine.handle_state_changes(deltaTime);

        // Obtener estado actual
        auto& currentState = stateMachine.getCurrentState();

        // Actualizar estado actual
        currentState->handleInput();
        currentState->update(deltaTime);
        currentState->render();
    }
    
    CloseAudioDevice();

    // Cleanup recursos
    ResourceManager::GetInstance().UnloadAll();

    CloseWindow();

    return 0;
}