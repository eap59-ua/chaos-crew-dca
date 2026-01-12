# Instrucciones para crear la Release Final - Hito 3

## Versión: v1.1.0

**Historial de versiones:**

- Hito 1: `v0.1.0-alpha`
- Hito 2: `v1.0.0` hasta `v1.0.4`
- **Hito 3: `v1.1.0`** ← Esta entrega

---

## Paso 1: Actualizar tu rama local main

**✅ PR ya mergeado** - La rama `fix/i18n-windows-support` ya fue mergeada a `main`.

```bash
git checkout main
git pull origin main
```

## Paso 2: Crear el tag de la versión final

```bash
# Crear tag anotado con la versión v1.1.0
git tag -a v1.1.0 -m "Release final - Entrega Hito 3

Características principales:
- Juego completo con múltiples niveles
- Soporte para 5 jugadores simultáneos (teclado + 4 gamepads)
- Internacionalización (Español/Inglés) con GNU gettext
- Interfaz de debug (F1) con ImGui
- Sistema de físicas
- Sistema de audio
- Gestión de estados (menú, gameplay, game over)
- Suite de tests con CTest
- Instaladores para Windows y Linux con CPack

Requisitos cumplidos:
✅ Gestión de estados (menú, gameplay, game over)
✅ Gráficos con sprites y gestor de contenido
✅ Mecánica principal (plataformas, enemigos, puertas)
✅ Empaquetado multiplataforma (Windows/Linux)
✅ Internacionalización (ES/EN)
✅ Suite de tests (10+ tests)
✅ CI/CD con GitHub Actions
✅ Patrón ECS con EnTT
✅ Logging con spdlog
✅ Interfaz de debug con ImGui
✅ Gestión de múltiples inputs (teclado + gamepads)
✅ Multijugador local (hasta 5 jugadores)"

# Subir el tag al repositorio remoto
git push origin v1.1.0
```

## Paso 3: Generar los instalables

### En Windows (MSYS2):

```bash
# Limpiar build anterior
rm -rf build_windows
mkdir build_windows
cd build_windows

# Configurar y compilar
cmake .. -G "MinGW Makefiles" -DCMAKE_BUILD_TYPE=Release
cmake --build . --config Release

# Generar instalables
cpack -G ZIP
# cpack -G NSIS  # Si tienes NSIS instalado para crear .exe
```

### En Linux/WSL:

```bash
# Limpiar build anterior
rm -rf build
mkdir build
cd build

# Configurar y compilar
cmake .. -DCMAKE_BUILD_TYPE=Release
cmake --build . --config Release

# Generar instalables
cpack -G TGZ
# cpack -G DEB  # Para crear .deb
```

Los instalables se generarán en los directorios `build/` o `build_windows/`.

## Paso 4: Crear la Release en GitHub

1. Ve a: https://github.com/eap59-ua/chaos-crew-dca/releases/new

2. Selecciona el tag: **`v1.1.0`**

3. Título de la release: **Chaos Crew - Versión Final v1.1.0 (Hito 3)**

4. Descripción (copia esto):

````markdown
# 🎮 Chaos Crew - Entrega Final (Hito 3)

Juego de plataformas 2D multijugador desarrollado en C++ con Raylib para la asignatura de Desarrollo Colaborativo de Aplicaciones.

## 🎯 Características Principales

- **Multijugador Local**: Hasta 5 jugadores simultáneos (1 teclado + 4 gamepads)
- **Internacionalización**: Soporte completo para Español e Inglés
- **Interfaz de Debug**: Presiona F1 para acceder al panel de desarrollo
- **Múltiples Niveles**: Sistema de progresión con diferentes desafíos
- **Físicas Realistas**: Sistema de colisiones y movimiento fluido
- **Audio Inmersivo**: Música de fondo y efectos de sonido

## 🎮 Controles

### Teclado (Jugador 1)

- **Movimiento**: Flechas ← →
- **Saltar**: Espacio
- **Cambiar idioma**: L
- **Debug**: F1

### Gamepad (Jugadores 2-5)

- **Movimiento**: Stick izquierdo / D-Pad
- **Saltar**: Botón A (Xbox) / X (PlayStation)
- Detección automática al conectar

## 📦 Instalación

### Windows

1. Descarga `chaos-crew-windows-v1.1.0.zip`
2. Extrae el archivo
3. Ejecuta `chaos-crew.exe`

### Linux

1. Descarga `chaos-crew-linux-v1.1.0.tar.gz`
2. Extrae: `tar -xzf chaos-crew-linux-v1.1.0.tar.gz`
3. Ejecuta: `./chaos-crew`

**Nota WSL**: Si usas WSL, asegúrate de tener configurado el locale:

```bash
export LANG=es_ES.UTF-8
export LC_ALL=es_ES.UTF-8
```
````

## 🛠️ Requisitos del Sistema

### Windows

- Windows 10/11
- Visual C++ Redistributable 2015-2022
- DirectX 9.0c o superior

### Linux

- Ubuntu 20.04+ / Debian 11+ (o equivalente)
- libintl (gettext)
- OpenGL 3.3+

## 🏗️ Compilar desde Código Fuente

Ver [`README.md`](README.md) para instrucciones detalladas de compilación.

## 📋 Requisitos Cumplidos (Hito 3)

- ✅ Gestión de estados (menú, gameplay, game over)
- ✅ Gráficos con sprites y gestor de contenido multimedia
- ✅ Mecánica principal de plataformas
- ✅ **Empaquetado multiplataforma (Windows/Linux) con CPack**
- ✅ **Internacionalización (Español/Inglés) con GNU gettext**
- ✅ **Suite de tests (10+ tests) con CTest**
- ✅ CI/CD con GitHub Actions
- ✅ Patrón ECS con EnTT
- ✅ Sistema de logging con spdlog
- ✅ Interfaz de debug con ImGui
- ✅ Gestión de múltiples inputs (teclado + gamepads)
- ✅ Multijugador local (hasta 5 jugadores)

## 📝 Notas de la Versión

Esta es la **entrega final del Hito 3**, que incluye:

1. **Instaladores multiplataforma**: Generados con CPack para Windows y Linux
2. **Localización completa**: Sistema de traducción con GNU gettext
3. **Suite de tests**: Más de 10 tests automatizados con CTest
4. **Documentación completa**: README actualizado con instrucciones de compilación e instalación

## 🎓 Equipo de Desarrollo

Proyecto desarrollado para la asignatura de Desarrollo Colaborativo de Aplicaciones (DCA) - Universidad de Alicante

## 📄 Licencia

Ver archivo LICENSE en el repositorio.

---

**¡Disfruta del juego!** 🎮✨

````

5. **Adjuntar archivos**: Arrastra y suelta los instalables generados:
   - `chaos-crew-windows-v1.1.0.zip` (Windows)
   - `chaos-crew-linux-v1.1.0.tar.gz` (Linux)
   - Cualquier otro formato (.deb, .exe, etc.)

6. Marca como **"Set as the latest release"** ✅

7. Haz clic en **"Publish release"**

## Paso 5: Preparar entrega para Moodle

Crea un archivo comprimido con:

```bash
# Desde la raíz del proyecto
tar -czf chaos-crew-entrega-hito3.tar.gz \
  --exclude='.git' \
  --exclude='build' \
  --exclude='build_windows' \
  --exclude='.vscode' \
  --exclude='.claude' \
  .

# O en Windows con 7-Zip/WinRAR:
# - Selecciona todos los archivos EXCEPTO .git, build, build_windows, .vscode, .claude
# - Comprimir como chaos-crew-entrega-hito3.zip
````

Dentro del comprimido debe haber:

1. **Código fuente** (todo el repositorio limpio)
2. **Instalables** (los .zip/.tar.gz generados con CPack)
3. **Enlace a itch.io** (crear archivo `ITCH_IO_LINK.txt` con la URL)

## Paso 6: Publicar en itch.io

Ver [`ENTREGA_FINAL.md`](ENTREGA_FINAL.md) sección "Publicar en itch.io" para instrucciones detalladas.

---

## Resumen de comandos rápidos

```bash
# 1. Actualizar main
git checkout main
git pull origin main

# 2. Crear y subir tag v1.1.0
git tag -a v1.1.0 -m "Release final - Entrega Hito 3"
git push origin v1.1.0

# 3. Generar instalables (ver arriba según SO)

# 4. Crear release en GitHub con los instalables

# 5. Preparar entrega para Moodle
```

**Fecha límite**: 12 de enero de 2026 - 23:59

---

## Historial de Versiones

| Hito       | Tag                 | Estado        |
| ---------- | ------------------- | ------------- |
| Hito 1     | `v0.1.0-alpha`      | ✅ Finalizado |
| Hito 2     | `v1.0.0` - `v1.0.4` | ✅ Finalizado |
| **Hito 3** | **`v1.1.0`**        | 🚀 **ACTUAL** |
