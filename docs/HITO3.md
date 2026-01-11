# 📚 Hito 3 - Release Candidate

## 📋 Índice

- [Estado del Proyecto](#estado-del-proyecto)
- [Funcionalidades Implementadas](#funcionalidades-implementadas)
- [Internacionalización](#internacionalización)
- [Empaquetado y Distribución (CPack)](#empaquetado-y-distribución-cpack)
- [Sistema de Progreso](#sistema-de-progreso)
- [Gestión de Recursos](#gestión-de-recursos)
- [Pendiente](#pendiente)

---

## 🎯 Estado del Proyecto

**Versión:** v0.3.0-rc
**Fecha:** Enero 2025

### Checklist Hito 3

- [x] Sistema de progreso de niveles
- [x] ResourceManager para optimización
- [x] Integración con TinyXML2
- [x] **Internacionalización (ES/EN)** ✨
- [x] **CPack para empaquetado (DEB/ZIP)** 📦
- [ ] Debug UI
- [ ] Tests unitarios
- [ ] Publicación en Itch.io
- [ ] Sistema de sonido mejorado

---

## 🌍 Internacionalización

### Descripción General

Sistema completo de localización multiidioma implementado usando **GNU gettext**, el estándar de la industria para internacionalización (i18n).

### Idiomas Soportados

| Idioma | Código | Estado | Textos Traducidos |
|--------|--------|--------|-------------------|
| 🇪🇸 Español | `es_ES` | ✅ Completo | 18/18 |
| 🇬🇧 Inglés | `en_US` | ✅ Completo | 18/18 |

### Uso en el Juego

#### Detección Automática

El juego detecta automáticamente el idioma del sistema operativo al iniciar:

```cpp
// En main.cpp
const char* sysLang = std::getenv("LANG");
if (sysLang && strstr(sysLang, "en_")) {
    defaultLang = "en_US";
} else {
    defaultLang = "es_ES";
}
Locale::GetInstance().Init(defaultLang);
```

#### Cambio Manual de Idioma

Durante el juego, presionar la tecla **L** cambia entre idiomas:

```
Tecla L → Español ↔ Inglés (tiempo real, sin reiniciar)
```

### Implementación Técnica

#### Arquitectura

```
Código Fuente (C++)
    └─> _("texto a traducir")
          └─> Locale::GetInstance().T()
                └─> gettext()
                      └─> chaos-crew.mo (binario compilado)
```

#### Componentes Principales

**1. Clase Locale (Singleton)**

Ubicación: [`src/locale/Locale.hpp`](../src/locale/Locale.hpp) y [`Locale.cpp`](../src/locale/Locale.cpp)

```cpp
class Locale {
public:
    static Locale& GetInstance();
    void Init(const std::string& defaultLang = "es_ES");
    void SetLanguage(const std::string& lang);
    std::string GetCurrentLanguage() const;
    const char* T(const char* msgid) const;

private:
    std::string currentLang;
    bool initialized;
};
```

**Características:**
- Patrón Singleton para acceso global
- Inicialización única al inicio del programa
- Cambio de idioma en runtime sin reinicio
- Soporte para detección automática del sistema

**2. Macro de Traducción**

```cpp
#define _(String) Locale::GetInstance().T(String)
```

**Uso en el código:**

```cpp
#include "locale/Locale.hpp"

// Antes:
DrawText("Press ENTER to Play", x, y, size, color);

// Después:
DrawText(_("Press ENTER to Play"), x, y, size, color);
```

**3. Archivos de Traducción**

Estructura de directorios:

```
po/
├── chaos-crew.pot              # Plantilla (template)
├── es_ES/
│   └── LC_MESSAGES/
│       ├── chaos-crew.po       # Traducción español (texto)
│       └── chaos-crew.mo       # Traducción español (binario)
└── en_US/
    └── LC_MESSAGES/
        ├── chaos-crew.po       # Traducción inglés (texto)
        └── chaos-crew.mo       # Traducción inglés (binario)
```

**Formato de archivos .po:**

```gettext
# Comentario
msgid "Press ENTER to Play"
msgstr "Presiona ENTER para Jugar"
```

**Archivos .mo:**
- Versión binaria compilada de los .po
- Generados automáticamente por CMake usando `msgfmt`
- Optimizados para carga rápida en runtime

#### Integración con CMake

En [`CMakeLists.txt`](../CMakeLists.txt:133-171):

```cmake
find_package(Gettext)

if(GETTEXT_FOUND)
    # Compilar .po → .mo para español
    add_custom_command(
        OUTPUT "${CMAKE_BINARY_DIR}/po/es_ES/LC_MESSAGES/chaos-crew.mo"
        COMMAND ${GETTEXT_MSGFMT_EXECUTABLE} -o ...
        DEPENDS "${CMAKE_SOURCE_DIR}/po/es_ES/LC_MESSAGES/chaos-crew.po"
        COMMENT "Compilando traducción español (es_ES)"
    )

    # Compilar .po → .mo para inglés
    add_custom_command(
        OUTPUT "${CMAKE_BINARY_DIR}/po/en_US/LC_MESSAGES/chaos-crew.mo"
        ...
    )

    # Target para compilar todas las traducciones
    add_custom_target(translations ALL
        DEPENDS
            "${CMAKE_BINARY_DIR}/po/es_ES/LC_MESSAGES/chaos-crew.mo"
            "${CMAKE_BINARY_DIR}/po/en_US/LC_MESSAGES/chaos-crew.mo"
    )
endif()
```

**Proceso de compilación:**

1. CMake detecta gettext instalado
2. Por cada idioma, compila `.po` → `.mo`
3. Copia archivos `.mo` al directorio de build
4. Los archivos quedan disponibles en `build/bin/po/`

### Textos Localizados

#### Desglose por Pantalla

**MainMenuState** (5 textos):
- Título del juego
- Instrucciones de navegación
- Estados de niveles ([LOCKED], [DONE])
- Mensaje de error (no hay mapas)

**GameplayState** (7 textos):
- HUD: Título y versión
- Controles de jugadores
- Objetivo cooperativo
- Mecánica troll (salida movida)
- Mensajes de victoria/derrota

**GameOverState** (4 textos):
- Mensaje de nivel completado
- Mensaje de game over
- Instrucciones para reintentar
- Instrucciones para volver al menú

**PauseState** (3 textos):
- Título "PAUSA"
- Opciones del menú (Continuar, Reiniciar, Salir)

#### Tabla Comparativa

| Original (msgid) | Español (es_ES) | Inglés (en_US) |
|------------------|-----------------|----------------|
| `CHAOS CREW` | CHAOS CREW | CHAOS CREW |
| `Choose a Level (UP/DOWN, ENTER). R = refresh` | Elige un Nivel (ARRIBA/ABAJO, ENTER). R = actualizar | Choose a Level (UP/DOWN, ENTER). R = refresh |
| `[LOCKED]` | [BLOQUEADO] | [LOCKED] |
| `[DONE]` | [COMPLETADO] | [DONE] |
| `P1: Arrows | P2: WASD` | J1: Flechas | J2: WASD | P1: Arrows | P2: WASD |
| `COOPERATIVE: Both must reach EXIT!` | COOPERATIVO: ¡Ambos deben llegar a la SALIDA! | COOPERATIVE: Both must reach EXIT! |
| `VICTORY!` | ¡VICTORIA! | VICTORY! |
| `DEFEAT!` | ¡DERROTA! | DEFEAT! |
| `PAUSA` | PAUSA | PAUSE |
| `Continuar` | Continuar | Continue |

### Añadir Nuevos Textos

#### Paso 1: Marcar en el Código

```cpp
#include "locale/Locale.hpp"

void render() {
    DrawText(_("Nuevo texto aquí"), x, y, size, color);
}
```

#### Paso 2: Actualizar Plantilla `.pot`

Editar [`po/chaos-crew.pot`](../po/chaos-crew.pot):

```gettext
msgid "Nuevo texto aquí"
msgstr ""
```

#### Paso 3: Traducir en Archivos `.po`

**Español** ([`po/es_ES/LC_MESSAGES/chaos-crew.po`](../po/es_ES/LC_MESSAGES/chaos-crew.po)):

```gettext
msgid "Nuevo texto aquí"
msgstr "Nuevo texto aquí"
```

**Inglés** ([`po/en_US/LC_MESSAGES/chaos-crew.po`](../po/en_US/LC_MESSAGES/chaos-crew.po)):

```gettext
msgid "Nuevo texto aquí"
msgstr "New text here"
```

#### Paso 4: Recompilar

```bash
cmake --build build
```

Los archivos `.mo` se regeneran automáticamente.

### Añadir Nuevo Idioma (ej: Francés)

#### 1. Crear Estructura de Directorios

```bash
mkdir -p po/fr_FR/LC_MESSAGES
```

#### 2. Copiar Plantilla

```bash
cp po/chaos-crew.pot po/fr_FR/LC_MESSAGES/chaos-crew.po
```

#### 3. Traducir

Editar `po/fr_FR/LC_MESSAGES/chaos-crew.po` con traducciones al francés.

#### 4. Actualizar CMakeLists.txt

Añadir compilación para francés:

```cmake
add_custom_command(
    OUTPUT "${CMAKE_BINARY_DIR}/po/fr_FR/LC_MESSAGES/chaos-crew.mo"
    COMMAND ${CMAKE_COMMAND} -E make_directory "${CMAKE_BINARY_DIR}/po/fr_FR/LC_MESSAGES"
    COMMAND ${GETTEXT_MSGFMT_EXECUTABLE} -o "${CMAKE_BINARY_DIR}/po/fr_FR/LC_MESSAGES/chaos-crew.mo"
            "${CMAKE_SOURCE_DIR}/po/fr_FR/LC_MESSAGES/chaos-crew.po"
    DEPENDS "${CMAKE_SOURCE_DIR}/po/fr_FR/LC_MESSAGES/chaos-crew.po"
    COMMENT "Compilando traducción francés (fr_FR)"
)
```

#### 5. Modificar main.cpp

```cpp
const char* sysLang = std::getenv("LANG");
if (sysLang) {
    if (strstr(sysLang, "en_")) defaultLang = "en_US";
    else if (strstr(sysLang, "fr_")) defaultLang = "fr_FR";  // Añadir
    else defaultLang = "es_ES";
}
```

### Herramientas Útiles

#### Validar Archivos .po

```bash
msgfmt -c -v -o /dev/null po/es_ES/LC_MESSAGES/chaos-crew.po
msgfmt -c -v -o /dev/null po/en_US/LC_MESSAGES/chaos-crew.po
```

#### Extraer Textos del Código (regenerar .pot)

```bash
xgettext --language=C++ --keyword=_ --output=po/chaos-crew.pot src/**/*.cpp
```

#### Editor GUI: Poedit

Recomendado para editar archivos `.po` con interfaz gráfica:

- **Descarga:** https://poedit.net/
- **Características:** Validación automática, búsqueda de textos sin traducir, estadísticas

### Troubleshooting

#### Las traducciones no aparecen

**Síntoma:** El juego muestra textos originales aunque el idioma esté configurado.

**Causas posibles:**

1. **Los archivos .mo no existen**
   ```bash
   # Verificar:
   ls build/po/*/LC_MESSAGES/*.mo

   # Si no existen, recompilar:
   cmake --build build
   ```

2. **El directorio de traducciones no se copia**
   ```bash
   # Verificar que están en el directorio de ejecución:
   ls build/bin/po/*/LC_MESSAGES/*.mo
   ```

3. **Los msgid no coinciden exactamente**
   - Revisar que el texto en `_()` es **idéntico** al `msgid` en el `.po`
   - Gettext distingue mayúsculas/minúsculas y espacios

#### Caracteres especiales (ñ, á, etc.) aparecen mal

**Solución:** Asegurarse que los archivos `.po` están guardados con codificación **UTF-8**.

En el header del `.po`:

```gettext
"Content-Type: text/plain; charset=UTF-8\n"
```

#### Error: "Gettext no encontrado"

**Solución:** Instalar gettext en el sistema:

```bash
# Ubuntu/Debian
sudo apt install gettext

# macOS
brew install gettext

# Windows (MSYS2)
pacman -S gettext
```

### Documentación Adicional

- [📚 Guía Completa de Internacionalización](INTERNATIONALIZATION.md)
- [🔗 GNU gettext Manual](https://www.gnu.org/software/gettext/manual/)
- [🔗 CMake FindGettext](https://cmake.org/cmake/help/latest/module/FindGettext.html)

---

## 📦 Empaquetado y Distribución (CPack)

### Descripción General

Sistema de empaquetado automático implementado con **CPack** (parte de CMake) para generar instaladores profesionales multiplataforma.

### Formatos Soportados

| Formato | Plataforma | Extensión | Estado |
|---------|-----------|-----------|--------|
| **DEB** | Debian/Ubuntu Linux | `.deb` | ✅ Completo |
| **ZIP** | Windows/Genérico | `.zip` | ✅ Completo |

### Características del Paquete

#### Paquete DEB (Linux)
- **Dependencias automáticas**: `libc6`, `libstdc++6`, `libtinyxml2-9`
- **Recomendaciones**: `gettext` (para i18n)
- **Instalación estándar**: `/usr/bin/`, `/usr/share/`
- **Integración con sistema**: Compatible con `apt`, `dpkg`
- **Desinstalación limpia**: `sudo apt remove chaos-crew`

#### Paquete ZIP (Windows/Genérico)
- **Portátil**: No requiere instalación
- **Autocontenido**: Incluye todas las dependencias estáticas
- **Estructura preservada**: Ejecutable + assets + traducciones

### Contenido de los Paquetes

Todos los paquetes incluyen:

```
chaos-crew/
├── bin/chaos-crew                    # Ejecutable del juego
├── share/
│   ├── chaos-crew/
│   │   ├── assets/                   # Gráficos y sonidos
│   │   │   ├── fonts/
│   │   │   ├── images/
│   │   │   ├── sounds/
│   │   │   └── sprites/
│   │   └── mapas/                    # Niveles del juego (6 mapas)
│   │       ├── mapa0.xml
│   │       ├── mapa1.xml
│   │       └── ...
│   └── locale/                       # Traducciones
│       ├── es_ES/LC_MESSAGES/chaos-crew.mo
│       └── en_US/LC_MESSAGES/chaos-crew.mo
└── share/doc/chaos-crew/             # Documentación
    ├── README.md
    └── HITO3.md
```

### Generación de Paquetes

#### Método Automático (Recomendado)

Usa el script `package.sh` para generar ambos paquetes automáticamente:

```bash
./package.sh
```

Este script:
1. Limpia build anterior
2. Configura el proyecto con CMake
3. Compila el código fuente
4. Compila traducciones (.po → .mo)
5. Genera paquetes DEB y ZIP con CPack
6. Verifica el contenido

#### Método Manual

```bash
# Configurar
mkdir build && cd build
cmake ..

# Compilar
cmake --build . -j$(nproc)

# Generar paquetes
cpack

# Resultado: build/packages/
# - chaos-crew_0.2.0_amd64.deb
# - chaos-crew-0.2.0-Linux.zip
```

### Instalación

#### Linux (DEB)

```bash
# Instalar paquete
sudo dpkg -i chaos-crew_0.2.0_amd64.deb

# Si hay dependencias faltantes
sudo apt-get install -f

# Ejecutar juego
chaos-crew

# Desinstalar
sudo apt remove chaos-crew
```

#### Windows/Genérico (ZIP)

```bash
# Extraer
unzip chaos-crew-0.2.0-Linux.zip

# Ejecutar (desde directorio extraído)
cd chaos-crew-0.2.0-Linux
./bin/chaos-crew
```

### Configuración CPack en CMakeLists.txt

```cmake
# Información del paquete
set(CPACK_PACKAGE_NAME "chaos-crew")
set(CPACK_PACKAGE_VERSION "0.2.0")
set(CPACK_PACKAGE_VENDOR "Chaos Crew Team - Universidad de Alicante")
set(CPACK_PACKAGE_CONTACT "eap59@alu.ua.es")

# Generadores
set(CPACK_GENERATOR "DEB;ZIP")

# Componentes
set(CPACK_COMPONENTS_ALL Runtime Assets Translations Documentation)

# DEB específico
set(CPACK_DEBIAN_PACKAGE_DEPENDS "libc6 (>= 2.31), libstdc++6 (>= 10), libtinyxml2-9")
set(CPACK_DEBIAN_PACKAGE_SECTION "games")
```

### Verificación de Paquetes

#### Verificar contenido DEB

```bash
# Listar archivos del paquete
dpkg-deb -c chaos-crew_0.2.0_amd64.deb

# Ver información del paquete
dpkg-deb -I chaos-crew_0.2.0_amd64.deb

# Verificar dependencias
dpkg-deb -f chaos-crew_0.2.0_amd64.deb Depends
```

#### Verificar instalación

```bash
# Después de instalar, verificar archivos
dpkg -L chaos-crew

# Verificar traducciones instaladas
ls -la /usr/share/locale/*/LC_MESSAGES/chaos-crew.mo

# Verificar mapas instalados
ls -la /usr/share/chaos-crew/mapas/
```

### Solución de Problemas

#### DEB: Dependencias faltantes

```bash
# Si dpkg -i falla con dependencias
sudo apt-get install -f

# Instalar dependencias manualmente
sudo apt-get install libc6 libstdc++6 libtinyxml2-9 gettext
```

#### i18n no funciona en paquete instalado

```bash
# Verificar que traducciones están instaladas
ls /usr/share/locale/es_ES/LC_MESSAGES/chaos-crew.mo
ls /usr/share/locale/en_US/LC_MESSAGES/chaos-crew.mo

# Reinstalar paquete si faltan
sudo dpkg -i --force-overwrite chaos-crew_0.2.0_amd64.deb
```

#### Ejecutable no encuentra recursos

El ejecutable busca recursos en este orden:
1. `/usr/share/chaos-crew/` (instalación sistema)
2. `../share/chaos-crew/` (relativo al ejecutable)
3. `./assets/` y `./mapas/` (directorio actual)

### Referencias

- **CMakeLists.txt**: Configuración completa de CPack (líneas 284-370)
- **package.sh**: Script automatizado de empaquetado
- [🔗 CPack Documentation](https://cmake.org/cmake/help/latest/module/CPack.html)
- [🔗 Debian Package Format](https://www.debian.org/doc/debian-policy/ch-controlfields.html)

---

## 📊 Sistema de Progreso

Sistema de seguimiento de niveles completados usando archivos JSON.

### Implementación

**Archivo:** [`src/utils/MapProgress.hpp/cpp`](../src/utils/)

```cpp
namespace MapProgress {
    void MarkCompleted(const std::string& mapPath);
    std::set<std::string> Load();
    std::vector<std::string> GetAvailableMaps();
}
```

**Persistencia:** `progress.json` en el directorio de ejecución

---

## 🎨 Gestión de Recursos

### ResourceManager

Sistema singleton para caché de texturas y prevención de cargas duplicadas.

**Archivo:** [`src/core/ResourceManager.h/cpp`](../src/core/ResourceManager.h)

**Características:**
- Caché automático de texturas
- Descarga centralizada de recursos
- Optimización de memoria

---

## 🔧 Compilación y Build

### Sistema de Build

**Herramienta:** CMake 3.15+

### Dependencias

- Raylib 5.0+
- TinyXML2
- GNU Gettext
- C++17

### Proceso de Compilación

```bash
cmake -S . -B build
cmake --build build
cd build/bin && ./chaos-crew
```

---

## 🚀 Pendiente para Release Final

### Alta Prioridad

- [ ] Debug UI para visualizar estado del juego
- [ ] CPack para generar instaladores (.deb, .exe)
- [ ] Tests unitarios (cobertura mínima 60%)
- [ ] Publicación en Itch.io

### Media Prioridad

- [ ] Sistema de sonido mejorado (volumen por canal)
- [ ] Más niveles (objetivo: 8 niveles)
- [ ] Menú de configuración (volumen, idioma, controles)

### Baja Prioridad

- [ ] Soporte para más idiomas (francés, alemán)
- [ ] Achievements/logros
- [ ] Leaderboards locales

---

## 📈 Métricas del Proyecto

### Código

- **Líneas de código:** ~3500
- **Archivos fuente:** 30+
- **Idiomas soportados:** 2
- **Textos traducidos:** 18 x 2 = 36

### Control de Versiones

- **Commits:** 50+
- **Branches:** GitHub Flow
- **Issues cerradas:** 15+

---

## 👥 Contribuciones al Hito 3

| Miembro | Contribución Principal |
|---------|----------------------|
| **Erardo Aldana** | Internacionalización, CMake, ResourceManager |
| **Ángel Gonjar** | Sistema de progreso, mecánicas de juego |
| **Rachid Mouradi** | UI/UX, menús |
| **Youssef S.A.** | Físicas, sistemas |
| **Darío S.D.** | Diseño de niveles |

---

**Última actualización:** Enero 2025
**Estado:** 🟢 En desarrollo activo
