# 📦 ENTREGA FINAL - Chaos Crew
## Fecha límite: 12 de enero de 2026

---

## ✅ CHECKLIST DE ENTREGA

### 1. Código Fuente ✅
- [x] Última versión de la rama `main`
- [x] Branch `fix/i18n-windows-support` listo para mergear
- **Acción**: Mergear el PR y descargar `main` actualizado

### 2. Instalables del Juego 🎮
**Generar con CPack en el directorio `build/` o `build_windows/`:**

#### Linux (DEB):
```bash
cd build
cpack -G DEB
# Genera: chaos-crew-0.2.0-Linux.deb
```

#### Windows (ZIP):
```bash
cd build_windows
cpack -G ZIP
# Genera: chaos-crew-0.2.0-Windows.zip
```

**Resultado esperado:**
- `chaos-crew-0.2.0-Linux.deb` (paquete Debian)
- `chaos-crew-0.2.0-Windows.zip` (portable Windows)

### 3. Publicación en Itch.io 🎨

**Responsable**: [NOMBRE DEL COMPAÑERO]

**Pasos para publicar:**

1. **Crear cuenta/proyecto en itch.io**
   - Ir a: https://itch.io/
   - Click en "Upload new project"
   - Título: `Chaos Crew`
   - URL: `chaos-crew` (ejemplo: usuario.itch.io/chaos-crew)

2. **Subir los instalables**
   - Subir `chaos-crew-0.2.0-Windows.zip`
   - Subir `chaos-crew-0.2.0-Linux.deb`
   - Marcar como "Downloadable"

3. **Configurar la página**
   - **Título**: Chaos Crew
   - **Subtitle**: Cooperative 2D Platformer
   - **Género**: Platformer, Multiplayer, Cooperative
   - **Tags**: multiplayer, coop, platformer, puzzle, 2d

4. **Descripción del juego** (usar esto):
   ```
   # 🎮 Chaos Crew

   A cooperative 2D platformer for 2-5 players where teamwork is essential!

   ## Features
   - 🤝 **2-5 Players**: 2 keyboard players + 3 gamepad players
   - 🌍 **Bilingual**: Spanish and English (press L to switch)
   - 🎮 **Gamepad Support**: Xbox controllers and PlayStation (DS4Windows)
   - 🛠️ **Debug Interface**: Press F1 for developer tools
   - 🗺️ **Multiple Levels**: 11 challenging cooperative levels

   ## Controls
   - **Player 1**: Arrow keys (move), ↑ (jump)
   - **Player 2**: WASD (move), W/Space (jump)
   - **Gamepads**: Auto-detected, left stick + A button
   - **F1**: Toggle debug UI
   - **L**: Change language
   - **ESC**: Pause

   ## Requirements
   - **Windows**: Windows 10+ (64-bit)
   - **Linux**: Ubuntu 20.04+ / Debian-based
   - **Controllers**: Optional, up to 3 gamepads

   ## Installation

   ### Windows
   1. Download `chaos-crew-0.2.0-Windows.zip`
   2. Extract the ZIP file
   3. Run `chaos-crew.exe`

   ### Linux
   1. Download `chaos-crew-0.2.0-Linux.deb`
   2. Install: `sudo dpkg -i chaos-crew-0.2.0-Linux.deb`
   3. Run from menu or: `chaos-crew`

   ## Credits
   Developed by Chaos Crew Team - Universidad de Alicante (DCA 2025)

   🚀 Built with Raylib, EnTT, and Dear ImGui
   ```

5. **Capturas de pantalla/GIF**
   - Tomar 3-5 capturas del juego en acción
   - Si es posible, un GIF de gameplay cooperativo
   - Mostrar la interfaz de debug (F1)

6. **Configuración de la página**
   - **Classification**: E (Everyone)
   - **Release status**: Released
   - **Pricing**: Free
   - **Community**: Habilitar comentarios

7. **Guardar y publicar**
   - Click en "Save & view page"
   - Verificar que todo se vea bien
   - Click en "Public" para hacer el juego visible

8. **Copiar el enlace**
   - Ejemplo: `https://usuario.itch.io/chaos-crew`
   - Guardar este enlace para la entrega

---

## 📄 PREPARAR ENTREGA PARA MOODLE

**Crear archivo comprimido con:**

```
chaos-crew-entrega-final.zip
├── codigo-fuente/
│   └── [Contenido completo del repositorio desde main]
├── instalables/
│   ├── chaos-crew-0.2.0-Windows.zip
│   └── chaos-crew-0.2.0-Linux.deb
└── ENLACE_ITCH_IO.txt (contiene la URL de itch.io)
```

### Comandos para preparar:

```bash
# 1. Clonar versión final de main
git clone https://github.com/eap59-ua/chaos-crew-dca.git chaos-crew-final
cd chaos-crew-final
git checkout main

# 2. Crear estructura de entrega
mkdir -p entrega-final/codigo-fuente
mkdir -p entrega-final/instalables

# 3. Copiar código fuente (sin build/, .git/)
rsync -av --exclude='.git' --exclude='build*' --exclude='.claude' . entrega-final/codigo-fuente/

# 4. Copiar instalables (generar con cpack primero)
cp build/packages/*.deb entrega-final/instalables/
cp build_windows/packages/*.zip entrega-final/instalables/

# 5. Crear archivo con enlace de itch.io
echo "https://usuario.itch.io/chaos-crew" > entrega-final/ENLACE_ITCH_IO.txt

# 6. Comprimir todo
cd entrega-final
zip -r ../chaos-crew-entrega-final.zip .
```

---

## 🎯 VALORACIÓN (según enunciado)

### Página de Itch.io
- ✅ Página personalizada con estética del juego
- ✅ Información detallada y clara
- ✅ Capturas de pantalla / GIFs
- ✅ Instaladores disponibles para descarga
- ✅ Instrucciones claras de instalación

### Aspectos Técnicos
- ✅ Producto jugable en Windows y Linux
- ✅ Instaladores funcionando correctamente
- ✅ Código fuente completo y compilable

---

## 📞 CONTACTO

**Responsable entrega Moodle**: [NOMBRE]
**Responsable itch.io**: [NOMBRE COMPAÑERO]
**Fecha límite**: 12 de enero de 2026 - 23:59

---

## 🚨 IMPORTANTE

- Solo entrega **UN miembro del grupo** en Moodle
- Indicar el resto de componentes en la entrega
- Verificar que los instalables funcionen antes de entregar
- Probar la página de itch.io antes de entregar el enlace

---

**Estado actual**:
- ✅ Código listo (PR pendiente de merge)
- ⏳ Instalables por generar
- ⏳ Publicación en itch.io pendiente
- ⏳ Entrega en Moodle pendiente
