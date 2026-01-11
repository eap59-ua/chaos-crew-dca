#!/bin/bash

# ==============================================================================
# Script helper para ejecutar tests de P03
# Uso: ./run_tests.sh [opción]
# ==============================================================================

# Colores para output
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
BLUE='\033[0;34m'
NC='\033[0m' # No Color

# Función para imprimir con color
print_color() {
    color=$1
    message=$2
    echo -e "${color}${message}${NC}"
}

# Función para mostrar ayuda
show_help() {
    print_color "$BLUE" "==================================="
    print_color "$BLUE" "  Tests P03 - Chaos Crew"
    print_color "$BLUE" "==================================="
    echo ""
    echo "Uso: ./run_tests.sh [opción]"
    echo ""
    echo "Opciones:"
    echo "  all              - Compilar y ejecutar todos los tests (por defecto)"
    echo "  build            - Solo compilar sin ejecutar"
    echo "  clean            - Limpiar build y recompilar todo"
    echo "  entity           - Solo tests de entidades (Tests 1-3)"
    echo "  map              - Solo tests de mapas (Tests 4-5)"
    echo "  state            - Solo tests de estados (Tests 6-7)"
    echo "  collision        - Solo tests de colisiones (Tests 8-9)"
    echo "  integration      - Solo test de integración (Test 10)"
    echo "  verbose          - Ejecutar con output detallado"
    echo "  help             - Mostrar esta ayuda"
    echo ""
    echo "Ejemplos:"
    echo "  ./run_tests.sh              # Ejecutar todos"
    echo "  ./run_tests.sh entity       # Solo entity_tests"
    echo "  ./run_tests.sh clean        # Limpiar y recompilar"
    echo ""
}

# Función para compilar
build_project() {
    print_color "$YELLOW" "🔨 Compilando proyecto..."
    
    if [ ! -d "build" ]; then
        mkdir build
    fi
    
    cd build
    cmake .. > /dev/null 2>&1
    
    if [ $? -ne 0 ]; then
        print_color "$RED" "❌ Error en configuración de CMake"
        exit 1
    fi
    
    cmake --build . > /dev/null 2>&1
    
    if [ $? -ne 0 ]; then
        print_color "$RED" "❌ Error en compilación"
        exit 1
    fi
    
    print_color "$GREEN" "✅ Compilación exitosa"
    cd ..
}

# Función para ejecutar tests
run_tests() {
    test_filter=$1
    verbose=$2
    
    cd build
    
    if [ -n "$test_filter" ]; then
        print_color "$BLUE" "🧪 Ejecutando tests: $test_filter"
        if [ "$verbose" = "true" ]; then
            ctest -R "$test_filter" --verbose
        else
            ctest -R "$test_filter" --output-on-failure
        fi
    else
        print_color "$BLUE" "🧪 Ejecutando todos los tests..."
        if [ "$verbose" = "true" ]; then
            ctest --verbose
        else
            ctest --output-on-failure
        fi
    fi
    
    result=$?
    cd ..
    
    if [ $result -eq 0 ]; then
        print_color "$GREEN" "✅ Todos los tests pasaron"
    else
        print_color "$RED" "❌ Algunos tests fallaron"
        exit 1
    fi
}

# Función para limpiar
clean_build() {
    print_color "$YELLOW" "🧹 Limpiando build..."
    rm -rf build
    print_color "$GREEN" "✅ Build limpiado"
}

# ==============================================================================
# MAIN
# ==============================================================================

# Si no hay argumentos, ejecutar todos los tests
if [ $# -eq 0 ]; then
    set -- "all"
fi

case "$1" in
    help)
        show_help
        ;;
    
    all)
        build_project
        run_tests "" false
        ;;
    
    build)
        build_project
        ;;
    
    clean)
        clean_build
        build_project
        run_tests "" false
        ;;
    
    entity)
        build_project
        run_tests "entity" false
        ;;
    
    map)
        build_project
        run_tests "map" false
        ;;
    
    state)
        build_project
        run_tests "state" false
        ;;
    
    collision)
        build_project
        run_tests "collision" false
        ;;
    
    integration)
        build_project
        run_tests "integration" false
        ;;
    
    verbose)
        build_project
        run_tests "" true
        ;;
    
    *)
        print_color "$RED" "❌ Opción desconocida: $1"
        echo ""
        show_help
        exit 1
        ;;
esac

exit 0
