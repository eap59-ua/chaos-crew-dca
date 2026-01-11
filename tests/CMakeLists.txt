# Tests configuration for Chaos Crew (P03 - 10 tests mínimos)
cmake_minimum_required(VERSION 3.15)

# ============================================================================
# PASO 1: Encontrar Boost.Test
# ============================================================================
# Boost.Test es el framework recomendado en el tema
find_package(Boost 1.71 REQUIRED COMPONENTS unit_test_framework)

# ============================================================================
# PASO 2: Configurar directorios de includes
# ============================================================================
include_directories(
    ${CMAKE_SOURCE_DIR}/include
    ${CMAKE_SOURCE_DIR}/src
    ${Boost_INCLUDE_DIRS}
)

# ============================================================================
# PASO 3: Función helper para crear tests
# ============================================================================
# Automatiza la creación de ejecutables de test
function(create_test test_name test_file)
    # Crear ejecutable del test
    add_executable(${test_name} ${test_file})
    
    # Linkear con dependencias
    target_link_libraries(${test_name}
        PRIVATE
        Boost::unit_test_framework  # Framework de testing
        EnTT::EnTT                  # Librería externa (Caso 6)
    )
    
    # Definir que usa Boost.Test dinámicamente
    target_compile_definitions(${test_name}
        PRIVATE
        BOOST_TEST_DYN_LINK
    )
    
    # ============ PASO CRÍTICO ============
    # Registrar test con CTest
    add_test(NAME ${test_name} COMMAND ${test_name})
    # ======================================
    
    # Configurar propiedades del test
    set_tests_properties(${test_name} PROPERTIES
        TIMEOUT 30                  # Timeout de 30 segundos
        WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
    )
endfunction()

# ============================================================================
# PASO 4: Crear los 10 tests seleccionados
# ============================================================================

# Tests 1-3: Sistema ECS (entity_tests.cpp)
create_test(entity_tests entity_tests.cpp)
set_tests_properties(entity_tests PROPERTIES LABELS "unit;ecs")

# Tests 4-5: Carga de mapas (map_tests.cpp)
create_test(map_tests map_tests.cpp)
set_tests_properties(map_tests PROPERTIES LABELS "unit;resources")

# Tests 6-7: Máquina de estados (state_machine_tests.cpp)
create_test(state_machine_tests state_machine_tests.cpp)
set_tests_properties(state_machine_tests PROPERTIES LABELS "unit;states")

# Tests 8-9: Colisiones (collision_tests.cpp)
create_test(collision_tests collision_tests.cpp)
set_tests_properties(collision_tests PROPERTIES LABELS "unit;physics")

# Test 10: Integración (integration_tests.cpp)
create_test(integration_tests integration_tests.cpp)
set_tests_properties(integration_tests PROPERTIES LABELS "integration")