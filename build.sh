#!/bin/bash

#############################################################################################
# build.sh - Script de compilación para Chaos Crew
# Uso: ./build.sh [linux|windows|clean|run|package]
#############################################################################################

set -e  # Exit on error

# Configuration
PROJECT_NAME="chaos-crew"
SRC_DIR="src"
BIN_DIR="bin"
BUILD_DIR="build"
RELEASE_DIR="releases"
VENDOR_DIR="vendor"

# Colors
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
BLUE='\033[0;34m'
CYAN='\033[0;36m'
NC='\033[0m' # No Color

# Banner
print_banner() {
    echo -e "${CYAN}"
    echo "╔═══════════════════════════════════════╗"
    echo "║     CHAOS CREW - Build System         ║"
    echo "║     DCA - Universidad de Alicante     ║"
    echo "╚═══════════════════════════════════════╝"
    echo -e "${NC}"
}

# Check if vendor directory exists
check_vendor() {
    if [ ! -d "$VENDOR_DIR" ]; then
        echo -e "${RED}❌ Error: Carpeta '$VENDOR_DIR' no encontrada${NC}"
        echo -e "${YELLOW}Por favor, copia la carpeta vendor/ del proyecto flappy-bird${NC}"
        echo -e "${YELLOW}Debe contener:${NC}"
        echo -e "  vendor/include/raylib.h"
        echo -e "  vendor/include/raymath.h"
        echo -e "  vendor/include/rlgl.h"
        echo -e "  vendor/lib/libraylib.a"
        exit 1
    fi
    
    if [ ! -f "$VENDOR_DIR/lib/libraylib.a" ]; then
        echo -e "${RED}❌ Error: libraylib.a no encontrado en vendor/lib/${NC}"
        exit 1
    fi
    
    if [ ! -f "$VENDOR_DIR/include/raylib.h" ]; then
        echo -e "${RED}❌ Error: raylib.h no encontrado en vendor/include/${NC}"
        exit 1
    fi
}

# Create directories
mkdir -p $BIN_DIR
mkdir -p $BUILD_DIR
mkdir -p $RELEASE_DIR

# Build for Linux
build_linux() {
    echo -e "${YELLOW}🔨 Compilando para Linux...${NC}"
    
    check_vendor
    
    g++ $SRC_DIR/main.cpp \
        -o $BIN_DIR/$PROJECT_NAME \
        -I$VENDOR_DIR/include \
        -L$VENDOR_DIR/lib \
        -lraylib -lGL -lm -lpthread -ldl -lrt -lX11 \
        -std=c++17 \
        -Wall -Wextra \
        -O2
    
    if [ $? -eq 0 ]; then
        echo -e "${GREEN}✅ Compilación exitosa!${NC}"
        echo -e "${GREEN}📦 Ejecutable: $BIN_DIR/$PROJECT_NAME${NC}"
        
        chmod +x $BIN_DIR/$PROJECT_NAME
        
        # Run option
        read -p "¿Ejecutar el juego ahora? (y/n) " -n 1 -r
        echo
        if [[ $REPLY =~ ^[Yy]$ ]]; then
            ./$BIN_DIR/$PROJECT_NAME
        fi
    else
        echo -e "${RED}❌ Error en la compilación${NC}"
        exit 1
    fi
}

# Build for Windows (cross-compilation with MinGW)
build_windows() {
    echo -e "${YELLOW}🔨 Compilando para Windows (cross-compilation)...${NC}"
    
    check_vendor
    
    # Check if MinGW is installed
    if ! command -v x86_64-w64-mingw32-g++ &> /dev/null; then
        echo -e "${RED}❌ MinGW no está instalado${NC}"
        echo -e "${YELLOW}Instalar con: sudo apt install mingw-w64${NC}"
        exit 1
    fi
    
    x86_64-w64-mingw32-g++ $SRC_DIR/main.cpp \
        -o $BIN_DIR/$PROJECT_NAME.exe \
        -I$VENDOR_DIR/include \
        -L$VENDOR_DIR/lib \
        -lraylib -lopengl32 -lgdi32 -lwinmm \
        -std=c++17 \
        -Wall -Wextra \
        -O2 \
        -static-libgcc -static-libstdc++
    
    if [ $? -eq 0 ]; then
        echo -e "${GREEN}✅ Compilación exitosa!${NC}"
        echo -e "${GREEN}📦 Ejecutable: $BIN_DIR/$PROJECT_NAME.exe${NC}"
    else
        echo -e "${RED}❌ Error en la compilación${NC}"
        exit 1
    fi
}

# Clean build artifacts
clean() {
    echo -e "${YELLOW}🧹 Limpiando archivos de compilación...${NC}"
    rm -rf $BIN_DIR/*
    rm -rf $BUILD_DIR/*
    echo -e "${GREEN}✅ Limpieza completada${NC}"
}

# Create release package
package() {
    VERSION=$1
    if [ -z "$VERSION" ]; then
        echo -e "${RED}❌ Especifica una versión: ./build.sh package v0.1.0-alpha${NC}"
        exit 1
    fi
    
    echo -e "${YELLOW}📦 Creando paquete de release ${VERSION}...${NC}"
    
    VERSION_DIR="$RELEASE_DIR/$VERSION"
    mkdir -p $VERSION_DIR
    
    # Build for both platforms
    echo -e "${CYAN}Compilando versión Linux...${NC}"
    build_linux <<< "n"  # Auto-responder "n" a la pregunta de ejecutar
    
    echo -e "${CYAN}Compilando versión Windows...${NC}"
    build_windows
    
    # Copy executables
    echo -e "${CYAN}Copiando archivos...${NC}"
    cp $BIN_DIR/$PROJECT_NAME $VERSION_DIR/${PROJECT_NAME}-linux
    cp $BIN_DIR/$PROJECT_NAME.exe $VERSION_DIR/ 2>/dev/null || echo "Windows build not available"
    
    # Copy assets if they exist and are not empty
    if [ -d "assets" ] && [ "$(ls -A assets 2>/dev/null)" ]; then
        cp -r assets $VERSION_DIR/
    fi
    
    # Copy documentation
    cp README.md $VERSION_DIR/ 2>/dev/null || echo "README.md not found"
    cp docs/HITO1.md $VERSION_DIR/HITO1.md 2>/dev/null || echo "HITO1.md not found"
    
    # Create ZIP for Linux
    echo -e "${CYAN}Creando archivo ZIP para Linux...${NC}"
    cd $RELEASE_DIR
    
    if [ -f "$VERSION/${PROJECT_NAME}-linux" ]; then
        zip -r ${PROJECT_NAME}-${VERSION}-linux.zip $VERSION/${PROJECT_NAME}-linux $VERSION/assets/ $VERSION/README.md $VERSION/HITO1.md 2>/dev/null || \
        zip -r ${PROJECT_NAME}-${VERSION}-linux.zip $VERSION/${PROJECT_NAME}-linux
    fi
    
    # Create ZIP for Windows
    if [ -f "$VERSION/${PROJECT_NAME}.exe" ]; then
        echo -e "${CYAN}Creando archivo ZIP para Windows...${NC}"
        zip -r ${PROJECT_NAME}-${VERSION}-windows.zip $VERSION/${PROJECT_NAME}.exe $VERSION/assets/ $VERSION/README.md $VERSION/HITO1.md 2>/dev/null || \
        zip -r ${PROJECT_NAME}-${VERSION}-windows.zip $VERSION/${PROJECT_NAME}.exe
    fi
    
    cd ../..
    
    echo -e "${GREEN}✅ Paquetes creados en $RELEASE_DIR/${NC}"
    ls -lh $RELEASE_DIR/${PROJECT_NAME}-${VERSION}-*.zip 2>/dev/null || echo "Packages created in $RELEASE_DIR/$VERSION/"
}

# Run the game
run() {
    if [ ! -f "$BIN_DIR/$PROJECT_NAME" ]; then
        echo -e "${YELLOW}Ejecutable no encontrado. Compilando primero...${NC}"
        build_linux <<< "n"
    fi
    
    echo -e "${CYAN}🎮 Ejecutando Chaos Crew...${NC}"
    ./$BIN_DIR/$PROJECT_NAME
}

# Show help
show_help() {
    echo "Uso: ./build.sh [comando]"
    echo ""
    echo "Comandos:"
    echo "  ${CYAN}linux${NC}          Compilar para Linux (por defecto)"
    echo "  ${CYAN}windows${NC}        Compilar para Windows (requiere MinGW)"
    echo "  ${CYAN}run${NC}            Ejecutar el juego (compila si es necesario)"
    echo "  ${CYAN}clean${NC}          Limpiar archivos de compilación"
    echo "  ${CYAN}package <ver>${NC}  Crear paquete de release (ej: v0.1.0-alpha)"
    echo "  ${CYAN}help${NC}           Mostrar esta ayuda"
    echo ""
    echo "Ejemplos:"
    echo "  ${YELLOW}./build.sh${NC}                    # Compilar para Linux"
    echo "  ${YELLOW}./build.sh run${NC}                # Compilar y ejecutar"
    echo "  ${YELLOW}./build.sh package v0.1.0-alpha${NC} # Crear release"
    echo ""
    echo "Requisitos:"
    echo "  - Carpeta vendor/ con raylib (copiada del proyecto flappy-bird)"
    echo "  - g++ instalado (Linux build)"
    echo "  - mingw-w64 instalado (Windows build - opcional)"
}

# Main execution
print_banner

case "${1:-linux}" in
    linux)
        build_linux
        ;;
    windows)
        build_windows
        ;;
    run)
        run
        ;;
    clean)
        clean
        ;;
    package)
        package $2
        ;;
    help|--help|-h)
        show_help
        ;;
    *)
        echo -e "${RED}❌ Comando desconocido: $1${NC}"
        show_help
        exit 1
        ;;
esac