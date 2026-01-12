#pragma once

#include <string>
#include <unordered_map>
#include "raylib.h"

/**
 * ResourceManager - Gestor centralizado de recursos (Singleton)
 *
 * Responsabilidades:
 * - Cargar y cachear texturas bajo demanda
 * - Cargar y cachear fuentes bajo demanda
 * - Evitar lecturas duplicadas del disco
 * - Gestionar la vida útil de los recursos
 *
 * Patrón: Singleton (una única instancia global)
 */
class ResourceManager {
public:
    // Obtener instancia única del ResourceManager (Singleton)
    static ResourceManager& GetInstance();

    /**
     * Obtener textura por ruta
     * - Si ya está cacheada, devuelve la referencia inmediatamente
     * - Si no existe, la carga con LoadTexture() y la cachea
     * @param path Ruta del archivo de textura (ej: "assets/sprites/player.png")
     * @return Referencia a la textura cacheada
     */
    Texture2D& GetTexture(const std::string& path);

    /**
     * Obtener fuente por ruta
     * - Si ya está cacheada, devuelve la referencia inmediatamente
     * - Si no existe, la carga con LoadFont() y la cachea
     * @param path Ruta del archivo de fuente (ej: "assets/fonts/game.ttf")
     * @param fontSize Tamaño de fuente en píxeles (por defecto: 32)
     * @return Referencia a la fuente cacheada
     */
    Font& GetFont(const std::string& path, int fontSize = 32);

    /**
     * Descargar todos los recursos
     * - Libera memoria de todas las texturas cacheadas
     * - Libera memoria de todas las fuentes cacheadas
     * - Limpia los caches internos
     *
     * Se debe llamar antes de CloseWindow() en main.cpp
     */
    void UnloadAll();
    /**
     * Gestión de música global
     */
    // Carga y reproduce la música de fondo global
    void PlayGlobalMusic(const std::string& path);
    
    // Debe llamarse en el bucle principal (main.cpp) frame a frame
    void UpdateGlobalMusic();

    // Ajustar volumen (0.0 a 1.0)
    void SetGlobalVolume(float volume);

    // Prevenir copias del Singleton
    ResourceManager(const ResourceManager&) = delete;
    ResourceManager& operator=(const ResourceManager&) = delete;

private:
    // Constructor privado (Singleton)
    ResourceManager() = default;

    // Destructor privado (limpia recursos)
    ~ResourceManager();

    // Caches internos
    std::unordered_map<std::string, Texture2D> textureCache;
    std::unordered_map<std::string, Font> fontCache;

    Music globalMusic = {0}; // Almacena la música actual
    bool isMusicLoaded = false;
};

/* EJEMPLO DE USO:

// En cualquier parte del código del juego:

// Obtener y usar una textura (se carga solo la primera vez)
auto& playerTexture = ResourceManager::GetInstance().GetTexture("assets/sprites/player.png");
DrawTexture(playerTexture, x, y, WHITE);

// Las siguientes veces que pidas la misma textura, devuelve la cacheada
auto& sameTexture = ResourceManager::GetInstance().GetTexture("assets/sprites/player.png");
// No se lee del disco, se usa la versión en memoria

// Obtener una fuente
auto& font = ResourceManager::GetInstance().GetFont("assets/fonts/game.ttf", 24);
DrawTextEx(font, "Hello World", {100, 100}, 24, 1, WHITE);

// Al final del programa (en main.cpp, antes de CloseWindow()):
ResourceManager::GetInstance().UnloadAll();
*/
