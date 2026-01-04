#include "Locale.hpp"
#include <libintl.h>
#include <locale.h>
#include <cstdlib>
#include <cstring>
#include <iostream>

// Nombre del dominio de traducción (debe coincidir con el nombre de los archivos .mo)
#define TEXTDOMAIN "chaos-crew"

// Ruta donde se encuentran los archivos .mo
// La estructura debe ser: po/es_ES/LC_MESSAGES/chaos-crew.mo
#define LOCALEDIR "po"

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

    // 1. Configurar el locale del sistema
    setlocale(LC_ALL, "");

    // 2. Especificar el directorio de traducciones
    bindtextdomain(TEXTDOMAIN, LOCALEDIR);

    // 3. Especificar la codificación de caracteres
    bind_textdomain_codeset(TEXTDOMAIN, "UTF-8");

    // 4. Establecer el dominio de texto por defecto
    textdomain(TEXTDOMAIN);

    // 5. Establecer el idioma inicial
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
    bindtextdomain(TEXTDOMAIN, LOCALEDIR);
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
