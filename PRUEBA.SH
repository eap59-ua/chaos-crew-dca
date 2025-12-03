#!/bin/bash

echo "=== SECCIÓN 3: Sin vs Con ccache ==="
echo ""

# Sin ccache
echo "1. SIN ccache:"
export CCACHE_DISABLE=1
make clean > /dev/null 2>&1
time make -j4 2>&1 | tail -3
TIEMPO_SIN=$(time make -j4 2>&1 | grep real | awk '{print $2}')
unset CCACHE_DISABLE
echo ""

# Con ccache (1ª vez)
echo "2. CON ccache (1ª compilación - cache miss):"
export CXX="ccache g++"
ccache -C > /dev/null 2>&1
ccache -z > /dev/null 2>&1
make clean > /dev/null 2>&1
time make -j4 2>&1 | tail -3
ccache -s | grep -E "Hits|Misses|Cache size"
echo ""

# Con ccache (2ª vez)
echo "3. CON ccache (2ª compilación - cache hit):"
make clean > /dev/null 2>&1
time make -j4 2>&1 | tail -3
ccache -s | grep -E "Hits|Misses|Cache size"
echo ""

echo "=== SECCIÓN 4: Estadísticas detalladas ==="
echo ""

# Limpiar y primera compilación
ccache -C > /dev/null 2>&1
ccache -z > /dev/null 2>&1
make clean > /dev/null 2>&1
echo "PRIMERA COMPILACIÓN:"
time make -j4 > /dev/null 2>&1
ccache -s
echo ""

# Segunda compilación
make clean > /dev/null 2>&1
echo "SEGUNDA COMPILACIÓN:"
time make -j4 > /dev/null 2>&1
ccache -s
echo ""

# Limpiar
unset CXX