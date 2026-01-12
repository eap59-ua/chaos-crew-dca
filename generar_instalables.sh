#!/bin/bash
# Script para generar instalables de Chaos Crew
# Ejecutar desde la raíz del proyecto

echo "=========================================="
echo "  GENERACIÓN DE INSTALABLES - Chaos Crew"
echo "=========================================="
echo ""

# Colores
GREEN='\033[0;32m'
RED='\033[0;31m'
YELLOW='\033[1;33m'
NC='\033[0m'

# ==========================================
# WINDOWS (MSYS2)
# ==========================================
echo -e "${YELLOW}1. Generando instalable para Windows...${NC}"

if [ -d "build_windows" ]; then
    cd build_windows

    # Compilar si no existe el ejecutable
    if [ ! -f "bin/chaos-crew.exe" ]; then
        echo "Compilando proyecto..."
        cmake --build . -j$(nproc) || exit 1
    fi

    # Generar ZIP
    echo "Creando paquete ZIP..."
    cpack -G ZIP

    if [ $? -eq 0 ]; then
        echo -e "${GREEN}✓ Instalable Windows generado: $(ls packages/*.zip)${NC}"
    else
        echo -e "${RED}✗ Error al generar instalable Windows${NC}"
    fi

    cd ..
else
    echo -e "${YELLOW}! build_windows no existe, saltando...${NC}"
fi

echo ""

# ==========================================
# LINUX
# ==========================================
echo -e "${YELLOW}2. Generando instalable para Linux...${NC}"

if [ -d "build" ]; then
    cd build

    # Compilar si no existe el ejecutable
    if [ ! -f "bin/chaos-crew" ]; then
        echo "Compilando proyecto..."
        cmake --build . -j$(nproc) || exit 1
    fi

    # Generar DEB
    echo "Creando paquete DEB..."
    cpack -G DEB

    if [ $? -eq 0 ]; then
        echo -e "${GREEN}✓ Instalable Linux generado: $(ls packages/*.deb)${NC}"
    else
        echo -e "${RED}✗ Error al generar instalable Linux${NC}"
    fi

    cd ..
else
    echo -e "${YELLOW}! build no existe, compilando primero...${NC}"
    mkdir build
    cd build
    cmake .. -DCMAKE_BUILD_TYPE=Release
    cmake --build . -j$(nproc)
    cpack -G DEB
    cd ..
fi

echo ""
echo "=========================================="
echo -e "${GREEN}✓ Proceso completado${NC}"
echo "=========================================="
echo ""
echo "Instalables generados en:"
[ -d "build_windows/packages" ] && echo "  - Windows: build_windows/packages/"
[ -d "build/packages" ] && echo "  - Linux: build/packages/"
echo ""
echo "Siguiente paso: Copiar instalables para la entrega"
