# Tests para HITO3 - Chaos Crew 
cmake_minimum_required(VERSION 3.15)

# Encontrar Boost (header-only, 
set(Boost_NO_BOOST_CMAKE ON)
find_package(Boost 1.71 REQUIRED)

message(STATUS "Boost encontrado: ${Boost_VERSION}")

# Función para crear tests 
function(create_test test_name test_file)
    add_executable(${test_name} ${test_file})
    
    target_include_directories(${test_name} PRIVATE
        ${CMAKE_SOURCE_DIR}/src
        ${CMAKE_SOURCE_DIR}/src/components
        ${CMAKE_SOURCE_DIR}/src/core
        ${CMAKE_SOURCE_DIR}/src/states
        ${CMAKE_SOURCE_DIR}/src/systems
        ${CMAKE_SOURCE_DIR}/src/utils
        ${Boost_INCLUDE_DIRS}
        ${CMAKE_SOURCE_DIR}/vendor/include
    )
    
    target_link_libraries(${test_name} PRIVATE
        ${CMAKE_SOURCE_DIR}/vendor/lib/libraylib.a)

    add_test(NAME ${test_name} COMMAND ${test_name})
    set_tests_properties(${test_name} PROPERTIES TIMEOUT 30)
endfunction()

# Crear los 10 tests 
create_test(entity_tests entity_tests.cpp)
create_test(map_tests map_tests.cpp)
create_test(state_machine_tests state_machine_tests.cpp)
create_test(collision_tests collision_tests.cpp)
create_test(integration_tests integration_tests.cpp)