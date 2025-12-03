# Hito 2: Sistema de Build Autoconfigurable

## Entregable 3: Proyecto Autoconfigurable con CMake

### Índice
1. [Sistema de Build Elegido](#1-sistema-de-build-elegido)
2. [Justificación de la Elección](#2-justificación-de-la-elección)
3. [Requisitos Previos](#3-requisitos-previos)
4. [Instrucciones de Compilación - Linux](#4-instrucciones-de-compilación---linux)
5. [Instrucciones de Compilación - Windows](#5-instrucciones-de-compilación---windows)
6. [Generación de Paquetes](#6-generación-de-paquetes)
7. [GitHub Actions - CI/CD](#7-github-actions---cicd)
8. [Troubleshooting](#8-troubleshooting)

---

## 1. Sistema de Build Elegido

**CMake 3.15+** - Sistema de construcción multiplataforma que genera archivos de proyecto nativos para diferentes herramientas de compilación.

### Características implementadas:
- ✅ Configuración automática de dependencias
- ✅ Detección de plataforma (Linux/Windows)
- ✅ Gestión automático de archivos fuente
- ✅ Copia automática de assets
- ✅ Integración con GitHub Actions para builds y empaquetado automático

---

## 2. Justificación de la Elección

### ¿Por qué CMake?

1. **Portabilidad multiplataforma**
   - Genera proyectos nativos para Visual Studio (Windows), Make (Linux), Ninja, etc.
   - Detecta automáticamente el compilador y adapta flags de compilación
   - Gestiona rutas de archivos de forma independiente del sistema operativo

2. **Gestión inteligente de dependencias**
   - Sistema `find_package()` para detectar librerías instaladas en el sistema
   - Fallback automático a librerías locales (vendor/)
   - Configuración de include paths y linking automática

3. **Escalabilidad y mantenibilidad**
   - Archivo único `CMakeLists.txt` vs múltiples Makefiles
   - Recopilación automática de archivos fuente con `GLOB_RECURSE`
   - Configuración declarativa y legible

4. **Herramientas integradas y soporte CI/CD**
   - **CTest**: Framework de testing integrado (para futuros tests)
   - **GNUInstallDirs**: Instalación compatible con estándares FHS de Linux
   - **GitHub Actions**: Integración nativa para builds automáticos y empaquetado

5. **Adopción en la industria**
   - Usado por proyectos grandes: LLVM, Qt, KDE, OpenCV
   - Amplio soporte en IDEs: Visual Studio, CLion, VS Code
   - Integración nativa con vcpkg (gestor de paquetes C++)

---

## 3. Requisitos Previos

### Linux (Ubuntu/Debian)
```bash
sudo apt-get update
sudo apt-get install -y \
    cmake \
    build-essential \
    libraylib-dev \
    git
```

### Windows
- **Visual Studio 2019/2022** con "Desktop development with C++"
- **CMake**: Descargar desde [cmake.org](https://cmake.org/download/)
- **vcpkg** (gestor de paquetes):
  ```powershell
  git clone https://github.com/microsoft/vcpkg.git
  cd vcpkg
  .\bootstrap-vcpkg.bat
  .\vcpkg integrate install
  .\vcpkg install raylib:x64-windows
  ```

---

## 4. Instrucciones de Compilación - Linux

### 4.1. Clonar el repositorio
```bash
git clone https://github.com/eap59-ua/chaos-crew-dca.git
cd chaos-crew-dca
```

### 4.2. Configurar y compilar
```bash
# Crear directorio de build (out-of-source build)
mkdir build
cd build

# Configurar el proyecto con CMake
cmake .. -DCMAKE_BUILD_TYPE=Release

# Compilar (usa todos los cores disponibles)
cmake --build . -j$(nproc)
```

### 4.3. Ejecutar el juego
```bash
# Desde el directorio build/
./bin/chaos-crew

# O desde la raíz del proyecto
cd ..
./bin/chaos-crew
```

### 4.4. Instalación en el sistema (opcional)
```bash
cd build
sudo cmake --install .
# El juego se instalará en /usr/local/bin/chaos-crew
chaos-crew
```

### 4.5. Desinstalación
```bash
cd build
sudo xargs rm < install_manifest.txt
```

---

## 5. Instrucciones de Compilación - Windows

### 5.1. Opción A: Con vcpkg (Recomendado)

```powershell
# Clonar repositorio
git clone https://github.com/eap59-ua/chaos-crew-dca.git
cd chaos-crew-dca

# Instalar Raylib con vcpkg (si no lo hiciste antes)
vcpkg install raylib:x64-windows

# Crear directorio de build
mkdir build
cd build

# Configurar con CMake (apuntando a vcpkg)
cmake .. -DCMAKE_BUILD_TYPE=Release `
         -DCMAKE_TOOLCHAIN_FILE="C:/ruta/a/vcpkg/scripts/buildsystems/vcpkg.cmake"

# Compilar
cmake --build . --config Release

# Ejecutar
.\bin\Release\chaos-crew.exe
```

### 5.2. Opción B: Con Raylib en vendor/ (sin vcpkg)

Si tienes Raylib en `vendor/lib/libraylib.a`:

```powershell
mkdir build
cd build
cmake .. -DCMAKE_BUILD_TYPE=Release
cmake --build . --config Release
.\bin\Release\chaos-crew.exe
```

### 5.3. Compilar desde Visual Studio

1. Abrir Visual Studio
2. **File → Open → CMake** → Seleccionar `CMakeLists.txt`
3. Visual Studio detectará automáticamente la configuración
4. **Build → Build All** (Ctrl+Shift+B)
5. **Debug → Start Without Debugging** (Ctrl+F5)

---

## 6. GitHub Actions - CI/CD y Generación de Paquetes

Los paquetes de distribución (.deb y .zip) se generan automáticamente mediante **GitHub Actions** al crear un release, no en el `CMakeLists.txt`.

### 6.1. Workflow de Release Automático

Ubicación: `.github/workflows/release.yml`

**Triggers:**
- ✅ Se ejecuta automáticamente al **crear un release** en GitHub
- ✅ Se puede ejecutar **manualmente** desde la pestaña "Actions"

**Jobs implementados:**

#### Job 1: `build-linux` (ubuntu-latest)
1. Instala dependencias: `libraylib-dev`, `cmake`, `build-essential`
2. Configura proyecto: `cmake .. -DCMAKE_BUILD_TYPE=Release`
3. Compila: `cmake --build .`
4. Genera paquete .deb: Usando herramientas nativas de Linux
5. **Sube artefacto**: `chaos-crew_0.2.0-beta_amd64.deb`
6. **Adjunta al release**: Automáticamente añade el .deb al release

#### Job 2: `build-windows` (windows-latest)
1. Instala Raylib con **vcpkg**: `vcpkg install raylib:x64-windows`
2. Configura proyecto con toolchain de vcpkg
3. Compila en modo Release
4. Genera paquete .zip: Comprimiendo los binarios y assets
5. **Sube artefacto**: `chaos-crew_0.2.0-beta_win64.zip`
6. **Adjunta al release**: Automáticamente añade el .zip al release

### 6.2. Cómo crear un release

```bash
# Desde GitHub Web UI:
1. Ir a "Releases" → "Create a new release"
2. Tag: v0.2.0-beta
3. Title: "Chaos Crew v0.2.0 Beta"
4. Descripción del release
5. "Publish release"

# El workflow se ejecuta automáticamente y adjunta los paquetes
```

### 6.3. Descargar artefactos manualmente

Si ejecutas el workflow manualmente (sin crear un release):
1. Ir a **Actions** en GitHub
2. Seleccionar el workflow run
3. Descargar:
   - `chaos-crew-linux-deb` (contiene el .deb)
   - `chaos-crew-windows-zip` (contiene el .zip)

---

## 8. Troubleshooting

### Problema 1: CMake no encuentra Raylib

**Síntoma:**
```
CMake Error: Could NOT find raylib (missing: raylib_DIR)
```

**Soluciones:**

**Linux:**
```bash
# Opción A: Instalar Raylib del sistema
sudo apt-get install libraylib-dev

# Opción B: Usar vendor/
# Asegúrate de que exista vendor/lib/libraylib.a y vendor/include/raylib.h
ls vendor/lib/libraylib.a
ls vendor/include/raylib.h
```

**Windows:**
```powershell
# Opción A: Instalar con vcpkg
vcpkg install raylib:x64-windows
vcpkg integrate install

# Opción B: Especificar ruta manualmente
cmake .. -Draylib_DIR="C:/ruta/a/raylib/lib/cmake/raylib"
```

---

### Problema 2: Error de linkado con librerías del sistema (Linux)

**Síntoma:**
```
undefined reference to `glXCreateContext'
/usr/bin/ld: cannot find -lGL
```

**Solución:**
```bash
# Instalar dependencias OpenGL y X11
sudo apt-get install -y \
    libgl1-mesa-dev \
    libx11-dev \
    libxrandr-dev \
    libxinerama-dev \
    libxcursor-dev \
    libxi-dev
```

---

### Problema 3: Assets no se copian al ejecutar el juego

**Síntoma:**
```
Error: No se puede cargar textura assets/sprites/player.png
```

**Diagnóstico:**
```bash
# Verificar si assets/ se copió a build/
ls build/assets/

# Si no existe, CMake no ejecutó la copia
```

**Solución:**
```bash
# Reconfigurar CMake (esto ejecuta file(COPY))
cd build
cmake ..

# O copiar manualmente
cp -r ../assets .

# Para Windows PowerShell:
Copy-Item -Path ..\assets -Destination . -Recurse
```

**Prevención:**
- Los assets se copian automáticamente durante:
  1. **Configuración**: `cmake ..` (con `file(COPY)`)
  2. **Compilación**: Cada `make`/`cmake --build` (con `POST_BUILD`)

---

## Archivos del Sistema de Build

```
chaos-crew-dca/
├── CMakeLists.txt                  # Configuración principal de CMake
├── .github/
│   └── workflows/
│       └── release.yml              # GitHub Actions workflow
├── src/                             # Código fuente (auto-detectado)
├── assets/                          # Assets (auto-copiados)
├── vendor/                          # Librerías locales (Raylib)
├── build/                           # Directorio de build (no en git)
│   ├── bin/chaos-crew              # Ejecutable compilado
│   └── assets/                      # Assets copiados
└── docs/
    └── HITO2.md                     # Esta documentación
```

---

## Ventajas del Sistema Implementado

| Característica | Antes (Makefile) | Ahora (CMake) |
|----------------|------------------|---------------|
| Multiplataforma | ❌ Solo Linux | ✅ Linux + Windows |
| Auto-detección deps | ❌ Manual | ✅ find_package() |
| Paquetes de distribución | ❌ No había | ✅ GitHub Actions |
| CI/CD | ❌ No había | ✅ GitHub Actions |
| Out-of-source build | ❌ Mezcla archivos | ✅ build/ separado |
| IDEs compatibles | ❌ Solo terminal | ✅ VS Code, Visual Studio, CLion |

---

## Referencias

- [CMake Official Documentation](https://cmake.org/documentation/)
- [GitHub Actions - Uploading Release Assets](https://docs.github.com/en/actions/automating-builds-and-tests/building-and-testing-cplusplus)
- [vcpkg - C++ Package Manager](https://vcpkg.io/)

---

**Desarrollado por:** Chaos Crew Team - Universidad de Alicante
**Fecha:** Noviembre 2024
**Versión:** 0.2.0-beta
