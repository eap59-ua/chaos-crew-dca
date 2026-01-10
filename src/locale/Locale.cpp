#include "Locale.hpp"
#include <libintl.h>
#include <locale.h>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <filesystem>

// Platform-specific includes para obtener ruta del ejecutable
#ifdef _WIN32
    #include <windows.h>
    #define PATH_MAX MAX_PATH
#else
    #include <unistd.h>
    #include <linux/limits.h>
#endif

namespace fs = std::filesystem;

// Nombre del dominio de traducción (debe coincidir con el nombre de los archivos .mo)
#define TEXTDOMAIN "chaos-crew"

/**
 * @brief Obtiene la ruta del directorio donde está el ejecutable
 * @return Ruta absoluta del directorio del ejecutable
 */
static std::string GetExecutablePath() {
#ifdef _WIN32
    char buffer[MAX_PATH];
    GetModuleFileNameA(NULL, buffer, MAX_PATH);
    return fs::path(buffer).parent_path().string();
#else
    char buffer[PATH_MAX];
    ssize_t len = readlink("/proc/self/exe", buffer, sizeof(buffer) - 1);
    if (len != -1) {
        buffer[len] = '\0';
        return fs::path(buffer).parent_path().string();
    }
    return ".";  // Fallback al directorio actual
#endif
}

/**
 * @brief Determina la ruta donde buscar las traducciones
 * Busca en orden:
 * 1. Instalación del sistema (/usr/share/locale en Linux)
 * 2. Relativo al ejecutable (../share/locale para instalador)
 * 3. Relativo al build (po/ para desarrollo)
 * @return Ruta al directorio de traducciones
 */
static std::string GetLocalePath() {
    std::string exePath = GetExecutablePath();

#ifdef _WIN32
    // Windows: buscar relativo al ejecutable
    // Estructura esperada: bin/chaos-crew.exe y share/locale/
    std::string localePath = exePath + "/../share/locale";

    if (fs::exists(localePath)) {
        return fs::canonical(localePath).string();
    }

    // Fallback: directorio po/ relativo (para desarrollo)
    localePath = exePath + "/po";
    if (fs::exists(localePath)) {
        return fs::canonical(localePath).string();
    }

    // Último fallback
    return "po";
#else
    // Linux: intentar primero la instalación del sistema
    if (fs::exists("/usr/share/locale")) {
        return "/usr/share/locale";
    }

    // Si no está instalado en el sistema, buscar relativo al ejecutable
    std::string localePath = exePath + "/../share/locale";
    if (fs::exists(localePath)) {
        return fs::canonical(localePath).string();
    }

    // Fallback: directorio po/ relativo (para desarrollo)
    localePath = exePath + "/po";
    if (fs::exists(localePath)) {
        return fs::canonical(localePath).string();
    }

    // Último fallback
    return "po";
#endif
}

Locale& Locale::GetInstance() {
    static Locale instance;
    return instance;
}

void Locale::Init(const std::string& defaultLang) {
    if (initialized) {
        std::cerr << "[Locale] Warning: Already initialized, skipping..." << std::endl;
        return;
    }

    currentLang = defaultLang;

    // 1. Obtener ruta de traducciones
    std::string localePath = GetLocalePath();
    std::cout << "[Locale] Using locale directory: " << localePath << std::endl;

    // 2. Configurar el locale del sistema
    setlocale(LC_ALL, "");

    // 3. Especificar el directorio de traducciones
    const char* bindResult = bindtextdomain(TEXTDOMAIN, localePath.c_str());
    if (bindResult) {
        std::cout << "[Locale] bindtextdomain: " << bindResult << std::endl;
    } else {
        std::cerr << "[Locale] Error: bindtextdomain failed!" << std::endl;
    }

    // 4. Especificar la codificación de caracteres
    bind_textdomain_codeset(TEXTDOMAIN, "UTF-8");

    // 5. Establecer el dominio de texto por defecto
    const char* domainResult = textdomain(TEXTDOMAIN);
    if (domainResult) {
        std::cout << "[Locale] textdomain: " << domainResult << std::endl;
    }

    // 6. Establecer el idioma inicial
    SetLanguage(defaultLang);

    initialized = true;

    std::cout << "[Locale] Initialized with language: " << currentLang << std::endl;
}

void Locale::SetLanguage(const std::string& lang) {
    if (!initialized) {
        std::cerr << "[Locale] Error: Not initialized, call Init() first!" << std::endl;
        return;
    }

    currentLang = lang;

    // Método multiplataforma para cambiar idioma en runtime:
    // Configurar variable de entorno LANGUAGE (tiene prioridad sobre LANG)

#ifdef _WIN32
    // En Windows usamos _putenv
    std::string envVar = "LANGUAGE=" + lang;
    _putenv(envVar.c_str());
#else
    // En Linux/macOS usamos setenv
    setenv("LANGUAGE", lang.c_str(), 1);
#endif

    // Refrescar el locale
    setlocale(LC_ALL, "");

    // Reforzar el binding (por si acaso)
    std::string localePath = GetLocalePath();
    bindtextdomain(TEXTDOMAIN, localePath.c_str());
    bind_textdomain_codeset(TEXTDOMAIN, "UTF-8");
    textdomain(TEXTDOMAIN);

    std::cout << "[Locale] Language changed to: " << lang << std::endl;
}

std::string Locale::GetCurrentLanguage() const {
    return currentLang;
}

const char* Locale::T(const char* msgid) const {
    if (!initialized) {
        // Si no está inicializado, devolver el texto original
        return msgid;
    }

    // gettext devuelve el msgid original si no encuentra traducción
    return gettext(msgid);
}
