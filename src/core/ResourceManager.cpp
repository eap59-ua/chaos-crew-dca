#include "ResourceManager.h"
#include <iostream>

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
    std::cout << "[ResourceManager] Cargando textura: " << path << std::endl;

    Texture2D texture = LoadTexture(path.c_str());

    // Verificar si la carga fue exitosa
    if (texture.id == 0) {
        std::cerr << "[ResourceManager] ERROR: No se pudo cargar la textura: " << path << std::endl;
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
    std::cout << "[ResourceManager] Cargando fuente: " << path << " (tamaño: " << fontSize << ")" << std::endl;

    Font font = LoadFontEx(path.c_str(), fontSize, nullptr, 0);

    // Verificar si la carga fue exitosa
    if (font.baseSize == 0) {
        std::cerr << "[ResourceManager] ERROR: No se pudo cargar la fuente: " << path << std::endl;
    }

    // Guardar en cache e devolver referencia
    fontCache[cacheKey] = font;
    return fontCache[cacheKey];
}

// Descargar todos los recursos cacheados
void ResourceManager::UnloadAll() {
    std::cout << "[ResourceManager] Descargar todos los recursos..." << std::endl;

    // Descargar todas las texturas
    std::cout << "[ResourceManager] Descargando " << textureCache.size() << " texturas..." << std::endl;
    for (auto& pair : textureCache) {
        UnloadTexture(pair.second);
    }
    textureCache.clear();

    // Descargar todas las fuentes
    std::cout << "[ResourceManager] Descargando " << fontCache.size() << " fuentes..." << std::endl;
    for (auto& pair : fontCache) {
        UnloadFont(pair.second);
    }
    fontCache.clear();

    std::cout << "[ResourceManager] Todos los recursos han sido descargados." << std::endl;
}

// Destructor
ResourceManager::~ResourceManager() {
    // Llamar a UnloadAll en el destructor para limpiar automáticamente
    // cuando se destruya la instancia (al terminar el programa)
    UnloadAll();
}
