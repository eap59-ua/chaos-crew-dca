#include "ResourceManager.h"
#include <iostream>
#include "utils/Logger.hpp"

// Implementación del Singleton
ResourceManager& ResourceManager::GetInstance() {
    // Instancia estática local (lazy initialization)
    // Se crea solo una vez la primera vez que se llama
    static ResourceManager instance;
    return instance;
}

// Obtener textura del cache o cargarla si no existe
Texture2D& ResourceManager::GetTexture(const std::string& path) {
    // Buscar si la textura ya está cacheada
    auto it = textureCache.find(path);

    if (it != textureCache.end()) {
        // Textura encontrada en cache - devolverla sin cargar del disco
        return it->second;
    }

    // Textura no está cacheada - cargarla del disco
    LOG_DEBUG("[ResourceManager] Loading texture: {}", path);

    Texture2D texture = LoadTexture(path.c_str());

    // Verificar si la carga fue exitosa
    if (texture.id == 0) {
        LOG_ERROR("[ResourceManager] Failed to load texture: {}", path);
        // Devolver una textura vacía o default (riesgo: puede causar problemas)
        // En producción, se podría devolver una textura de error/placeholder
    }

    // Guardar en cache e devolver referencia
    textureCache[path] = texture;
    return textureCache[path];
}

// Obtener fuente del cache o cargarla si no existe
Font& ResourceManager::GetFont(const std::string& path, int fontSize) {
    // Crear clave única combinando path y tamaño
    std::string cacheKey = path + "_" + std::to_string(fontSize);

    // Buscar si la fuente ya está cacheada con este tamaño
    auto it = fontCache.find(cacheKey);

    if (it != fontCache.end()) {
        // Fuente encontrada en cache - devolverla sin cargar del disco
        return it->second;
    }

    // Fuente no está cacheada - cargarla del disco
    LOG_DEBUG("[ResourceManager] Loading font: {} (size: {})", path, fontSize);

    Font font = LoadFontEx(path.c_str(), fontSize, nullptr, 0);

    // Verificar si la carga fue exitosa
    if (font.baseSize == 0) {
        LOG_ERROR("[ResourceManager] Failed to load font: {}", path);
    }

    // Guardar en cache e devolver referencia
    fontCache[cacheKey] = font;
    return fontCache[cacheKey];
}
void ResourceManager::PlayGlobalMusic(const std::string& path) {
    // Si ya hay música cargada, la descargamos primero
    if (isMusicLoaded) {
        StopMusicStream(globalMusic);
        UnloadMusicStream(globalMusic);
        isMusicLoaded = false;
    }

    std::cout << "[ResourceManager] Cargando Musica Global: " << path << std::endl;
    globalMusic = LoadMusicStream(path.c_str());

    if (globalMusic.stream.buffer != nullptr) {
        isMusicLoaded = true;
        SetMusicVolume(globalMusic, 0.5f); // Volumen por defecto
        PlayMusicStream(globalMusic);
    } else {
        std::cerr << "[ResourceManager] ERROR: No se pudo cargar musica: " << path << std::endl;
    }
}

void ResourceManager::UpdateGlobalMusic() {
    if (isMusicLoaded) {
        UpdateMusicStream(globalMusic);
    }
}

void ResourceManager::SetGlobalVolume(float volume) {
    if (isMusicLoaded) {
        SetMusicVolume(globalMusic, volume);
    }
}
// Descargar todos los recursos cacheados
void ResourceManager::UnloadAll() {
    LOG_INFO("[ResourceManager] Unloading all resources...");

    // Descargar todas las texturas
    LOG_INFO("[ResourceManager] Unloading {} textures...", textureCache.size());
    for (auto& pair : textureCache) {
        UnloadTexture(pair.second);
    }
    textureCache.clear();

    // Descargar todas las fuentes
    LOG_INFO("[ResourceManager] Unloading {} fonts...", fontCache.size());
    for (auto& pair : fontCache) {
        UnloadFont(pair.second);
    }
    fontCache.clear();

    // Descargar música global si está cargada
    if (isMusicLoaded) {
        StopMusicStream(globalMusic);
        UnloadMusicStream(globalMusic);
        isMusicLoaded = false;
        std::cout << "[ResourceManager] Musica global descargada." << std::endl;
    }

    LOG_INFO("[ResourceManager] All resources unloaded");
}

// Destructor
ResourceManager::~ResourceManager() {
    // Llamar a UnloadAll en el destructor para limpiar automáticamente
    // cuando se destruya la instancia (al terminar el programa)
    UnloadAll();
}
