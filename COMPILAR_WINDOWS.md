# 🎮 Guía: Compilar Chaos Crew en Windows Nativo

**Problema**: WSL no puede detectar gamepads de Windows.
**Solución**: Compilar nativamente en Windows.

---

## 📦 Paso 1: Instalar Chocolatey

Chocolatey es un administrador de paquetes para Windows (como `apt` en Ubuntu).

1. **Abre PowerShell como Administrador**:

   - Presiona `Win + X`
   - Selecciona "Windows PowerShell (Administrador)"

2. **Ejecuta este comando**:

```powershell
Set-ExecutionPolicy Bypass -Scope Process -Force; [System.Net.ServicePointManager]::SecurityProtocol = [System.Net.ServicePointManager]::SecurityProtocol -bor 3072; iex ((New-Object System.Net.WebClient).DownloadString('https://community.chocolatey.org/install.ps1'))
```

3. **Verifica la instalación**:

```powershell
choco --version
```

Deberías ver algo como: `2.2.2` o similar.

---

## 🔧 Paso 2: Instalar Herramientas de Compilación

**En la misma PowerShell como Administrador**, ejecuta:

```powershell
# Instalar CMake
choco install cmake -y

# Instalar MinGW (compilador GCC para Windows)
choco install mingw -y
```

Esto tardará **2-5 minutos**. Espera a que termine.

---

## 🔄 Paso 3: Reiniciar PowerShell

**IMPORTANTE**: Cierra PowerShell y ábrelo de nuevo (sin necesidad de ser administrador ahora).

Verifica que las herramientas estén instaladas:

```powershell
cmake --version
g++ --version
```

Deberías ver las versiones instaladas.

---

## 🏗️ Paso 4: Compilar el Proyecto

**Abre PowerShell normal** (no como administrador) y ejecuta:

```powershell
# Navega al proyecto
cd C:\Users\erard\Documents\DCA\chaos-crew-dca

# Limpia build anterior de WSL (opcional pero recomendado)
Remove-Item -Recurse -Force build

# Nota: en Windows el proyecto compila Raylib desde fuentes para asegurar
# compatibilidad con tu MinGW (puede tardar un poco la primera vez).

# Configura el proyecto para MinGW
cmake -B build -G "MinGW Makefiles"

# Compila
cmake --build build

# Ejecuta el juego
.\build\bin\chaos-crew.exe
```

---

## 🎮 Paso 5: Probar el Gamepad

**ANTES de ejecutar el juego**:

1. ✅ Abre **DS4Windows**
2. ✅ Conecta tu **DualSense** (cable USB-C)
3. ✅ Verifica que DS4Windows lo detecte
4. ✅ Abre `joy.cpl` - debe aparecer "Xbox 360 Controller"
5. ✅ **AHORA** ejecuta: `.\build\bin\chaos-crew.exe`

---

## 🔍 Verificar Detección

El juego creará un archivo `chaos-crew.log`. Ábrelo y busca:

```
========== GAMEPAD DETECTION CHECK ==========
Gamepad 0 DETECTED: Xbox 360 Controller  ← ¡ESTO ES LO QUE QUEREMOS VER!
```

Si dice `DETECTED`, **¡funcionó!** 🎉

Si sigue diciendo `NOT DETECTED`, revisa:

- ¿DS4Windows está corriendo?
- ¿joy.cpl muestra "Xbox 360 Controller"?
- ¿Conectaste el control ANTES de abrir el juego?

---

## ⚠️ Problemas Comunes

### Error: "cmake: command not found"

- Reinicia PowerShell después de instalar
- O cierra y abre una nueva terminal

### Error: "g++: command not found"

- Verifica que MinGW se instaló: `choco list mingw`
- Reinstala: `choco uninstall mingw -y && choco install mingw -y`

### Error: `fatal error: libintl.h: No such file or directory`

Esto ocurre si no tienes **gettext/libintl** instalado.

- ✅ **Desde v0.2.x el proyecto compila igualmente**: la internacionalización se desactiva automáticamente y `_()` devuelve el texto original.
- Si quieres **traducciones reales** en Windows, instala gettext (MSYS2) y asegúrate de que `libintl.h` y `libintl` estén disponibles:
  - MSYS2: `pacman -S gettext`

### Error al compilar Raylib

- Asegúrate de tener conexión a internet (CMake descarga Raylib)
- Si falla, borra `build` y vuelve a intentar

---

## 🚀 Compilación Futura

Una vez instaladas las herramientas, solo necesitas:

```powershell
cd C:\Users\erard\Documents\DCA\chaos-crew-dca
cmake --build build
.\build\bin\chaos-crew.exe
```

---

## 📝 Notas

- **WSL vs Windows**: Siempre compila en Windows para gamepads
- **Primer build**: Tarda más (descarga Raylib, spdlog, etc.)
- **Builds siguientes**: Solo 10-30 segundos

---

**¿Listo?** ¡Ejecuta los comandos y cuéntame qué sale! 🎮
