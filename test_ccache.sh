#!/bin/bash

echo "=== EXPERIMENTO CCACHE ==="
echo ""

# Limpiar ccache
ccache -C
ccache -z

# 1. Sin ccache (baseline)
echo "1. SIN CCACHE:"
make clean > /dev/null 2>&1
time make -j4 2>&1 | grep "^real"
echo ""

# 2. Con ccache - 1ª vez (cache miss)
echo "2. CON CCACHE (1ª compilación):"
ccache -C > /dev/null 2>&1
ccache -z > /dev/null 2>&1
make clean > /dev/null 2>&1
time make CXX="ccache g++" -j4 2>&1 | grep "^real"
echo ""
echo "Estadísticas:"
ccache -s
echo ""

# 3. Con ccache - 2ª vez (cache hit)
echo "3. CON CCACHE (2ª compilación):"
make clean > /dev/null 2>&1
time make CXX="ccache g++" -j4 2>&1 | grep "^real"
echo ""
echo "Estadísticas:"
ccache -s
echo ""