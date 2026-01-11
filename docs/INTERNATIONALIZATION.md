# 🌐 Internacionalización (i18n) - Chaos Crew

## 📋 Resumen

Sistema de internacionalización implementado usando **GNU gettext** que permite al juego cambiar entre Español e Inglés dinámicamente.

## ✅ Idiomas Soportados

- 🇪🇸 **Español** (`es_ES`) - Idioma por defecto
- 🇬🇧 **Inglés** (`en_US`)

## 🎮 Uso en el Juego

### Cambio de Idioma

Durante el juego, puedes cambiar el idioma presionando la tecla **L**:

```
Tecla L → Cambia entre Español ↔ Inglés
```

### Detección Automática

El juego detecta automáticamente el idioma del sistema al iniciar:
- Si tu sistema está en inglés (`LANG=en_*`), el juego inicia en inglés
- De lo contrario, inicia en español

## 🏗️ Arquitectura

### Estructura de Archivos

```
chaos-crew-dca/
├── src/locale/
│   ├── Locale.hpp          # Header del gestor de i18n
│   └── Locale.cpp          # Implementación del gestor
│
├── po/                      # Directorio de traducciones
│   ├── chaos-crew.pot       # Plantilla de traducción
│   ├── es_ES/
│   │   └── LC_MESSAGES/
│   │       ├── chaos-crew.po    # Traducción español (texto)
│   │       └── chaos-crew.mo    # Traducción español (binario)
│   └── en_US/
│       └── LC_MESSAGES/
│           ├── chaos-crew.po    # Traducción inglés (texto)
│           └── chaos-crew.mo    # Traducción inglés (binario)
```

### Componentes

#### 1. Clase `Locale` (Singleton)

Gestor centralizado de internacionalización:

```cpp
// Inicializar (se hace automáticamente en main.cpp)
Locale::GetInstance().Init("es_ES");

// Cambiar idioma
Locale::GetInstance().SetLanguage("en_US");

// Obtener idioma actual
std::string lang = Locale::GetInstance().GetCurrentLanguage();

// Traducir texto
const char* texto = Locale::GetInstance().T("Hello World");
```

#### 2. Macro `_()`

Atajo para traducir textos:

```cpp
#include "locale/Locale.hpp"

// En lugar de:
DrawText(Locale::GetInstance().T("Press ENTER to Play"), x, y, size, color);

// Simplemente usa:
DrawText(_("Press ENTER to Play"), x, y, size, color);
```

## 🔧 Compilación

### Requisitos

- **GNU gettext** instalado en el sistema
- CMake 3.15+

#### Instalar gettext

**Ubuntu/Debian:**
```bash
sudo apt install gettext
```

**macOS:**
```bash
brew install gettext
```

**Windows (MSYS2/MinGW):**
```bash
pacman -S gettext
```

### Compilar el Proyecto

```bash
# Configurar con CMake
cmake -S . -B build

# Compilar
cmake --build build

# Los archivos .mo se generan automáticamente durante la compilación
```

### Verificar Traducciones Compiladas

```bash
ls build/po/*/LC_MESSAGES/*.mo

# Debería mostrar:
# build/po/es_ES/LC_MESSAGES/chaos-crew.mo
# build/po/en_US/LC_MESSAGES/chaos-crew.mo
```

## ✏️ Añadir/Modificar Traducciones

### 1. Marcar Texto para Traducción

En el código fuente:

```cpp
#include "locale/Locale.hpp"

void render() {
    DrawText(_("Nuevo texto aquí"), x, y, size, color);
}
```

### 2. Actualizar Plantilla `.pot`

Edita `po/chaos-crew.pot` y añade la nueva entrada:

```gettext
msgid "Nuevo texto aquí"
msgstr ""
```

### 3. Actualizar Traducciones `.po`

**Español** (`po/es_ES/LC_MESSAGES/chaos-crew.po`):

```gettext
msgid "Nuevo texto aquí"
msgstr "Nuevo texto aquí"
```

**Inglés** (`po/en_US/LC_MESSAGES/chaos-crew.po`):

```gettext
msgid "Nuevo texto aquí"
msgstr "New text here"
```

### 4. Recompilar

Los archivos `.mo` se generan automáticamente:

```bash
cmake --build build
```

## 📊 Textos Localizados

### Menú Principal

| Original (msgid) | Español | Inglés |
|------------------|---------|--------|
| `CHAOS CREW` | CHAOS CREW | CHAOS CREW |
| `Choose a Level (UP/DOWN, ENTER). R = refresh` | Elige un Nivel (ARRIBA/ABAJO, ENTER). R = actualizar | Choose a Level (UP/DOWN, ENTER). R = refresh |
| `[LOCKED]` | [BLOQUEADO] | [LOCKED] |
| `[DONE]` | [COMPLETADO] | [DONE] |

### Gameplay

| Original (msgid) | Español | Inglés |
|------------------|---------|--------|
| `P1: Arrows | P2: WASD` | J1: Flechas | J2: WASD | P1: Arrows | P2: WASD |
| `COOPERATIVE: Both must reach EXIT!` | COOPERATIVO: ¡Ambos deben llegar a la SALIDA! | COOPERATIVE: Both must reach EXIT! |
| `The exit moved! Go back!` | ¡La salida se movió! ¡Regresa! | The exit moved! Go back! |
| `VICTORY!` | ¡VICTORIA! | VICTORY! |
| `DEFEAT!` | ¡DERROTA! | DEFEAT! |

### Menú de Pausa

| Original (msgid) | Español | Inglés |
|------------------|---------|--------|
| `PAUSA` | PAUSA | PAUSE |
| `Continuar` | Continuar | Continue |
| `Reiniciar nivel` | Reiniciar nivel | Restart level |
| `Salir del juego` | Salir del juego | Exit game |

## 🛠️ Herramientas Útiles

### Extraer Textos Automáticamente

Si quieres regenerar el `.pot` desde el código:

```bash
xgettext --language=C++ --keyword=_ --output=po/chaos-crew.pot src/**/*.cpp
```

### Validar Archivos `.po`

```bash
msgfmt -c -v -o /dev/null po/es_ES/LC_MESSAGES/chaos-crew.po
msgfmt -c -v -o /dev/null po/en_US/LC_MESSAGES/chaos-crew.po
```

### Editar con GUI

Puedes usar **Poedit** para editar archivos `.po` con interfaz gráfica:

- **Descargar:** https://poedit.net/

## 🐛 Troubleshooting

### Las traducciones no aparecen

**Problema:** El juego muestra textos en inglés aunque esté configurado en español.

**Solución:**
1. Verifica que los archivos `.mo` existen en `build/po/*/LC_MESSAGES/`
2. Asegúrate de ejecutar desde el directorio correcto:
   ```bash
   cd build/bin
   ./chaos-crew
   ```
3. Revisa que `po/` está copiado correctamente al directorio de ejecución

### Error: "Gettext no encontrado"

**Solución:** Instala gettext en tu sistema (ver sección Requisitos)

### Los textos tienen caracteres raros (ñ, á, etc.)

**Solución:** Asegúrate que los archivos `.po` están guardados con codificación UTF-8

## 📚 Referencias

- [GNU gettext Manual](https://www.gnu.org/software/gettext/manual/)
- [CMake FindGettext](https://cmake.org/cmake/help/latest/module/FindGettext.html)

## 👥 Contribuir

Para añadir un nuevo idioma (ej: Francés):

1. Crea directorio `po/fr_FR/LC_MESSAGES/`
2. Copia `chaos-crew.pot` → `po/fr_FR/LC_MESSAGES/chaos-crew.po`
3. Traduce los `msgstr` al francés
4. Añade la compilación en `CMakeLists.txt`
5. Modifica `Locale::SetLanguage()` para soportar `"fr_FR"`

---

**Implementado por:** Chaos Crew Team
**Fecha:** Enero 2025
**Versión:** 1.0
