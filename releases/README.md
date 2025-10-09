# 📦 Releases - Chaos Crew

Esta carpeta contiene las releases empaquetadas del juego Chaos Crew.

## 📋 Estructura de Releases

Cada release sigue el formato de **Versionado Semántico 2.0.0**:

```
vMAJOR.MINOR.PATCH-PRERELEASE

Ejemplos:
- v0.1.0-alpha    (Hito 1 - Alpha)
- v0.2.0-beta     (Hito 2 - Beta)
- v1.0.0          (Hito 3 - Release final)
```

## 🗂️ Contenido de una Release

Cada release incluye:

```
releases/
├── chaos-crew-v0.1.0-alpha-linux.zip
│   ├── chaos-crew-linux (ejecutable)
│   ├── assets/ (si hay)
│   ├── README.md
│   └── HITO1.md
│
└── chaos-crew-v0.1.0-alpha-windows.zip
    ├── chaos-crew.exe (ejecutable)
    ├── assets/ (si hay)
    ├── README.md
    └── HITO1.md
```

## 🚀 Cómo Crear una Release

### Opción 1: Script Automatizado (Recomendado)

```bash
# Desde la raíz del proyecto
chmod +x build.sh
./build.sh package v0.1.0-alpha
```

Esto generará automáticamente:

- Compilación para Linux
- Compilación para Windows (si MinGW está instalado)
- Archivos ZIP empaquetados en `releases/`

### Opción 2: Manual

```bash
# 1. Compilar ejecutables
./build.sh linux
./build.sh windows

# 2. Crear directorio de la versión
mkdir -p releases/v0.1.0-alpha

# 3. Copiar archivos
cp bin/chaos-crew releases/v0.1.0-alpha/chaos-crew-linux
cp bin/chaos-crew.exe releases/v0.1.0-alpha/
cp README.md releases/v0.1.0-alpha/
cp docs/HITO1.md releases/v0.1.0-alpha/

# 4. Empaquetar
cd releases
zip -r chaos-crew-v0.1.0-alpha-linux.zip v0.1.0-alpha/chaos-crew-linux
zip -r chaos-crew-v0.1.0-alpha-windows.zip v0.1.0-alpha/chaos-crew.exe
```

## 📤 Cómo Publicar en GitHub Releases

1. **Crear el tag:**

   ```bash
   git tag -a v0.1.0-alpha -m "Hito 1 - Alpha Release"
   git push origin v0.1.0-alpha
   ```

2. **Ir a GitHub:**

   - Navegar a: `https://github.com/eap59-ua/chaos-crew-dca/releases`
   - Click en "Draft a new release"

3. **Configurar la release:**

   - **Tag:** Seleccionar `v0.1.0-alpha`
   - **Title:** `Chaos Crew v0.1.0-alpha - Hito 1`
   - **Description:**

     ```markdown
     ## 🎮 Chaos Crew - Alpha Release (Hito 1)

     Primera versión jugable del juego cooperativo de plataformas.

     ### ✨ Características

     - ✅ 2 jugadores simultáneos
     - ✅ Nivel básico funcional
     - ✅ Mecánicas core de plataformas
     - ✅ Sistema de colisiones
     - ✅ Condiciones de victoria cooperativa

     ### 📥 Descarga

     - **Linux:** Descargar `chaos-crew-v0.1.0-alpha-linux.zip`
     - **Windows:** Descargar `chaos-crew-v0.1.0-alpha-windows.zip`

     ### 🎯 Controles

     - **Jugador 1:** A/D mover, W saltar
     - **Jugador 2:** Flechas mover, ↑ saltar

     ### 📚 Documentación

     Ver `HITO1.md` incluido en el paquete.

     ---

     **Equipo:** Chaos Crew - Universidad de Alicante DCA 2025
     ```

4. **Subir archivos:**

   - Arrastrar `chaos-crew-v0.1.0-alpha-linux.zip`
   - Arrastrar `chaos-crew-v0.1.0-alpha-windows.zip`
   - (Opcional) Adjuntar también el código fuente automáticamente

5. **Publicar:**
   - Si es una pre-release (alpha/beta): Marcar "Set as a pre-release"
   - Click "Publish release"

## 📖 Instrucciones para Usuarios

### Linux

```bash
# 1. Descargar y descomprimir
unzip chaos-crew-v0.1.0-alpha-linux.zip
cd v0.1.0-alpha

# 2. Dar permisos de ejecución
chmod +x chaos-crew-linux

# 3. Ejecutar
./chaos-crew-linux
```

### Windows

```
1. Descargar chaos-crew-v0.1.0-alpha-windows.zip
2. Extraer el archivo ZIP
3. Doble click en chaos-crew.exe
```

## 🔍 Verificación de Release

Antes de publicar, verificar:

- [ ] El ejecutable Linux funciona
- [ ] El ejecutable Windows funciona (si aplica)
- [ ] La documentación está incluida
- [ ] Los archivos ZIP no están corruptos
- [ ] El versionado es correcto
- [ ] El tag de Git está creado

## 📝 Notas

- **NO subir a Git:** Los archivos ZIP y ejecutables NO se suben al repositorio Git, solo se publican en GitHub Releases
- **Gitignore:** Los archivos `bin/`, `releases/*.zip` están en `.gitignore`
- **Espacio:** Las releases solo ocupan espacio en GitHub Releases, no en el repositorio

## 🆘 Solución de Problemas

### Error: "Permission denied" al ejecutar en Linux

```bash
chmod +x chaos-crew-linux
```

### Error: "libraylib.a not found" al compilar

Verificar que `vendor/lib/libraylib.a` existe. Si no, copiar del proyecto flappy-bird.

### Error: "cannot execute binary file" en Linux

Verificaste que descargaste la versión Linux, no Windows.

---

**Última actualización:** 9 de octubre de 2025  
**Equipo:** Chaos Crew - DCA UA
