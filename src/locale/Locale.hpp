#ifndef LOCALE_HPP
#define LOCALE_HPP

#include <string>

/**
 * @brief Singleton para gestionar internacionalización (i18n) usando GNU gettext
 *
 * Uso:
 *   1. Inicializar al inicio: Locale::GetInstance().Init("es_ES");
 *   2. Cambiar idioma: Locale::GetInstance().SetLanguage("en_US");
 *   3. Traducir texto: Locale::GetInstance().T("Hello World")
 *   4. O usar el macro: _("Hello World")
 */
class Locale {
public:
    /**
     * @brief Obtener instancia singleton
     * @return Referencia a la instancia única
     */
    static Locale& GetInstance();

    /**
     * @brief Inicializar sistema de localización
     * @param defaultLang Idioma por defecto (ej: "es_ES", "en_US")
     */
    void Init(const std::string& defaultLang = "es_ES");

    /**
     * @brief Cambiar idioma en tiempo de ejecución
     * @param lang Código de idioma (ej: "es_ES", "en_US")
     */
    void SetLanguage(const std::string& lang);

    /**
     * @brief Obtener idioma actual
     * @return Código del idioma activo
     */
    std::string GetCurrentLanguage() const;

    /**
     * @brief Traducir un texto (wrapper de gettext)
     * @param msgid Texto a traducir (clave)
     * @return Texto traducido al idioma actual
     */
    const char* T(const char* msgid) const;

private:
    Locale() = default;
    ~Locale() = default;

    // Prevenir copia y asignación (patrón singleton)
    Locale(const Locale&) = delete;
    Locale& operator=(const Locale&) = delete;

    std::string currentLang = "es_ES";
    bool initialized = false;
};

// Macro para traducción rápida: _("texto") en lugar de Locale::GetInstance().T("texto")
#define _(String) Locale::GetInstance().T(String)

#endif // LOCALE_HPP
