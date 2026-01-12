# 🎮 Chaos Crew - Cooperative Platformer

[![Release](https://img.shields.io/github/v/release/eap59-ua/chaos-crew-dca)](https://github.com/eap59-ua/chaos-crew-dca/releases)
[![Build Status](https://img.shields.io/badge/build-passing-brightgreen)]()
[![License](https://img.shields.io/badge/license-MIT-blue.svg)](LICENSE)

## 📖 Descripción

**Chaos Crew** es un juego de plataformas cooperativo 2D desarrollado en C++ con Raylib para la asignatura de Desarrollo Colaborativo de Aplicaciones de la Universidad de Alicante.

Inspirado en **Level Devil** (mecánicas de trampas dinámicas) y **Pico Park** (cooperación esencial), el juego desafía a 2-5 jugadores a superar niveles llenos de obstáculos impredecibles que requieren coordinación y trabajo en equipo.

## 🎯 Estado del Proyecto

**Versión actual:** v0.1.0-alpha (en desarrollo)  
**Fecha de entrega Hito 1:** 22 de octubre de 2025

### Roadmap

#### ✅ Hito 0 - Setup (1-2 Oct)

- [x] Propuesta validada por el profesor
- [x] Repositorio creado y configurado
- [x] Estructura de carpetas implementada
- [x] Estrategia de ramas definida
- [x] Sistema de bugtracking configurado

#### 🔄 Hito 1 - Alpha (3-22 Oct)

- [x] Pantalla de juego básica
- [x] Pantalla de Game Over
- [x] Mecánica de movimiento de 2 jugadores
- [x] Sistema básico de colisiones
- [x] Nivel funcional con objetivo
- [x] Bucle de juego completo
- [ ] Release v0.1.0-alpha

#### 📅 Hito 2 - Beta (TBD)

- [ ] Multijugador 3-5 jugadores
- [ ] Sistema de trampas dinámicas
- [ ] Mecánicas cooperativas avanzadas
- [ ] Sprites y audio básico
- [ ] Suite de tests (min. 10)
- [ ] CI/CD con GitHub Actions

#### 📅 Hito 3 - Release (TBD)

- [ ] 5-8 niveles completos
- [ ] Sistema ECS con EnTT
- [x] Internacionalización (ES/EN) ✨
- [ ] Ejecutables Linux y Windows
- [ ] Documentación completa

## 🚀 Instalación y Compilación

### Requisitos previos

- **C++ Compiler:** g++ (GCC) 9.0 o superior
- **CMake:** 3.15 o superior
- **Raylib:** v5.0 o superior
- **GNU Gettext:** Para soporte de internacionalización
- **Sistema Operativo:** Linux (Ubuntu 20.04+) o Windows 10+

### Instalar dependencias en Linux (Ubuntu/Debian)

```bash
sudo apt update
sudo apt install build-essential git cmake gettext
sudo apt install libasound2-dev libx11-dev libxrandr-dev libxi-dev \
                 libgl1-mesa-dev libglu1-mesa-dev libxcursor-dev \
                 libxinerama-dev libwayland-dev libxkbcommon-dev \
                 libtinyxml2-dev

# Clonar e instalar Raylib
git clone https://github.com/raysan5/raylib.git
cd raylib/src
make PLATFORM=PLATFORM_DESKTOP
sudo make install
cd ../..
```

### Compilar Chaos Crew

```bash
# Clonar el repositorio
git clone https://github.com/eap59-ua/chaos-crew-dca.git
cd chaos-crew-dca

# Compilar con CMake (recomendado)
cmake -S . -B build
cmake --build build

# Ejecutar (Linux/WSL)
cd build/bin
./chaos-crew

# O usar el script helper en WSL (configura locales automáticamente)
./run-wsl.sh
```

### Compilación alternativa (Makefile legacy)

```bash
# Dar permisos de ejecución
chmod +x build.sh

# Compilar y ejecutar
./build.sh linux
```

### Compilar en Windows (MSYS2)

**Prerequisite**: MSYS2 must be installed (see Windows Requirements section above)

```bash
# Open "MSYS2 MinGW 64-bit" terminal
cd /c/Users/YourUsername/path/to/chaos-crew-dca

# Create build directory
mkdir build_windows && cd build_windows

# Configure with CMake
cmake .. -G "MinGW Makefiles" -DCMAKE_BUILD_TYPE=Release

# Build the project
cmake --build . -j$(nproc)

# Run the game
./bin/chaos-crew.exe
```

**Note**: The game executable and all assets will be in the `build_windows/bin/` directory.

## 🌍 Multi-language Support

Chaos Crew supports multiple languages with automatic detection:

- 🇪🇸 **Spanish (Español)** - Default language
- 🇬🇧 **English** - Full translation

### Change Language

Press **L** during gameplay to toggle between Spanish and English.

The game automatically detects your system language at startup and selects the appropriate translation.

### Requirements for i18n (Linux/WSL)

On Linux/WSL, you need to have the required locales installed:

```bash
# 1. Install and generate locales
sudo apt install locales
sudo locale-gen es_ES.UTF-8
sudo locale-gen en_US.UTF-8
sudo update-locale

# 2. Configure default locale (permanent fix for WSL)
echo 'export LANG=es_ES.UTF-8' >> ~/.bashrc
echo 'export LC_ALL=es_ES.UTF-8' >> ~/.bashrc
source ~/.bashrc

# 3. Verify configuration
echo $LANG  # Should output: es_ES.UTF-8
```

**Troubleshooting for WSL**: If i18n doesn't work after the above steps, close and reopen your WSL terminal. Alternatively, run the game with explicit locale:
```bash
LANG=es_ES.UTF-8 LC_ALL=es_ES.UTF-8 ./bin/chaos-crew
```

### Windows Requirements

For Windows compilation with i18n support, **MSYS2** is required:

1. Download and install MSYS2 from https://www.msys2.org/
2. Open "MSYS2 MinGW 64-bit" terminal
3. Install dependencies:
   ```bash
   pacman -S mingw-w64-x86_64-gcc mingw-w64-x86_64-cmake mingw-w64-x86_64-make mingw-w64-x86_64-gettext mingw-w64-x86_64-tinyxml2
   ```
4. Compile the project (see Windows compilation section below)

For more details, see [Internationalization Documentation](docs/INTERNATIONALIZATION.md).

## 🎮 Controls

| Action                | Player 1  | Player 2 |
| --------------------- | --------- | -------- |
| Move left             | A         | ←        |
| Move right            | D         | →        |
| Jump                  | W / SPACE | ↑        |
| Restart (Game Over)   | ENTER     | ENTER    |
| Toggle Language       | L         | L        |
| **Toggle Debug UI**   | **F1**    | **F1**   |
| Pause                 | ESC       | ESC      |

## 🔧 Debug Interface

Chaos Crew includes a built-in debug interface powered by **Dear ImGui**:

- **Activation**: Press **F1** to toggle the debug panel
- **Features**:
  - Real-time FPS and frame time monitoring
  - Adjustable float parameters (gravity, speed, etc.)
  - Integer parameters (jump count, lives, etc.)
  - Boolean toggles for features
  - Action buttons for testing
- **Customization**: Improved contrast dark theme for better visibility
- **Use cases**: Level design, gameplay tuning, bug testing

The debug interface is automatically compiled into development builds and can be toggled on/off during runtime.

## 👥 Equipo de Desarrollo

| Nombre                   | Rol Principal         | GitHub                                                         | Responsabilidades                     |
| ------------------------ | --------------------- | -------------------------------------------------------------- | ------------------------------------- |
| **Erardo Aldana Pessoa** | Project Lead / DevOps | [@eap59-ua](https://github.com/eap59-ua)                       | Gestión del proyecto, CI/CD, releases |
| **Ángel Gonjar Verdejo** | Gameplay Programmer   | [@agv114-ua](https://github.com/agv114-ua)                     | Mecánicas de juego, controles         |
| **Rachid Mouradi**       | UI/Screens Developer  | [@mouradirachid8-lgtm](https://github.com/mouradirachid8-lgtm) | Interfaces, menús, HUD                |
| **Youssef S.A.**         | Systems Programmer    | [@ysa9-ua](https://github.com/ysa9-ua)                         | Físicas, colisiones, input manager    |
| **Darío S.D.**           | Level Designer        | [@dsd17-ua](https://github.com/dsd17-ua)                       | Diseño de niveles, trampas            |


## 📋 Gestión del Proyecto

- **Estrategia de ramas:** GitHub Flow (documentado en [docs/HITO1.md](docs/HITO1.md))
- **Bugtracking:** GitHub Issues + Projects
- **Versionado:** Semantic Versioning 2.0.0
- **CI/CD:** GitHub Actions (Hito 2+)

### Links importantes

- [📊 Project Board](https://github.com/eap59-ua/chaos-crew-dca/projects)
- [🐛 Issues](https://github.com/eap59-ua/chaos-crew-dca/issues)
- [📦 Releases](https://github.com/eap59-ua/chaos-crew-dca/releases)
- [📚 Documentación](docs/)

## 📦 Releases

### v0.1.0-alpha (22 Oct 2025) - Planeado

Primera versión jugable con mecánicas básicas y 2 pantallas.

- 2 jugadores simultáneos
- Nivel básico funcional
- Gráficos con figuras geométricas

## 📚 Documentación

- [GDD (Game Design Document)](docs/GDD.md)
- [Hito 1 - Documentación de entrega](docs/HITO1.md)
- [Estrategia de ramas](docs/HITO1.md#estrategia-de-gestión-de-ramas)
- [Sistema de bugtracking](docs/HITO1.md#sistema-de-bugtracking)

## 🐛 Reportar Bugs

Si encuentras un bug, [abre un issue](https://github.com/eap59-ua/chaos-crew-dca/issues/new?template=bug_report.md) usando la plantilla de Bug Report.

## 🤝 Contribuir

Este es un proyecto académico, pero si quieres contribuir:

1. Fork el proyecto
2. Crea una rama feature (`git checkout -b feature/AmazingFeature`)
3. Commit tus cambios (`git commit -m 'feat: Add some AmazingFeature'`)
4. Push a la rama (`git push origin feature/AmazingFeature`)
5. Abre un Pull Request

## 📄 Licencia

Este proyecto está bajo la Licencia MIT - ver el archivo [LICENSE](LICENSE) para más detalles.

## 🙏 Agradecimientos

- [Raylib](https://www.raylib.com/) - Framework de desarrollo de videojuegos
- Universidad de Alicante - Asignatura DCA
- **Inspiración:**
  - [Level Devil](https://www.crazygames.com/game/level-devil) - Mecánicas de trampas
  - [Pico Park](https://store.steampowered.com/app/1509960/PICO_PARK/) - Cooperación

## 📞 Contacto

- **Repositorio:** https://github.com/eap59-ua/chaos-crew-dca
- **Issues:** https://github.com/eap59-ua/chaos-crew-dca/issues
- **Profesor:** Antonio Ruiz Martínez (DCA - Universidad de Alicante)

---

**Desarrollado con ❤️ por el equipo de Chaos Crew**  
_Grado en Ingeniería Informática - Universidad de Alicante - 2025_



