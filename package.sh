#!/bin/bash
# Script para generar paquetes con CPack
# Chaos Crew - Universidad de Alicante

set -e  # Salir si hay error

echo "🎮 Chaos Crew - Generador de Paquetes"
echo "======================================"
echo ""

# Limpiar build anterior
if [ -d "build" ]; then
    echo "🧹 Limpiando build anterior..."
    rm -rf build
fi

mkdir build
cd build

# Configurar con CMake
echo "⚙️  Configurando proyecto con CMake..."
cmake ..

echo ""

# Compilar
echo "🔨 Compilando..."
cmake --build . -j$(nproc)

echo ""

# Verificar que las traducciones se compilaron
if [ -f "po/es_ES/LC_MESSAGES/chaos-crew.mo" ] && [ -f "po/en_US/LC_MESSAGES/chaos-crew.mo" ]; then
    echo "✅ Traducciones compiladas correctamente"
else
    echo "⚠️  WARNING: Archivos .mo no encontrados - i18n puede no funcionar"
fi

echo ""

# Generar paquetes con CPack
echo "📦 Generando paquetes..."
cpack

echo ""

# Mostrar resultados
echo "━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━"
echo "✅ PAQUETES GENERADOS:"
echo "━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━"
ls -lh packages/

echo ""
echo "📦 Archivos disponibles:"
find packages/ -type f \( -name "*.deb" -o -name "*.zip" \)

echo ""
echo "━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━"
echo "🎉 ¡Empaquetado completado!"
echo "━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━"
echo ""
echo "Para instalar el paquete DEB (Linux):"
echo "  sudo dpkg -i packages/chaos-crew-*.deb"
echo ""
echo "Para extraer el ZIP (Windows/Generic):"
echo "  unzip packages/chaos-crew-*.zip"
echo ""
