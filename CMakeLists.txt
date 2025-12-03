# ============================================================================
# CMakeLists.txt para Chaos Crew
# ============================================================================
# Equipo: 5 integrantes
# Asignatura: Desarrollo Colaborativo de Aplicaciones (DCA)
# Universidad de Alicante - 2025
# ============================================================================

cmake_minimum_required(VERSION 3.15)

# ============================================================================
# Información del proyecto
# ============================================================================
project(ChaosCrew
    VERSION 0.2.0
    DESCRIPTION "Chaos Crew - Juego cooperativo 2D de plataformas"
    LANGUAGES CXX
)

# ============================================================================
# Configuración del estándar C++
# ============================================================================
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# ============================================================================
# Incluir FetchContent desde el inicio
# ============================================================================
include(FetchContent)

# ============================================================================
# Opciones de compilación
# ============================================================================
if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
    add_compile_options(-Wall -Wextra -Wpedantic)
    set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O3")
    set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -g -O0")
elseif(MSVC)
    add_compile_options(/W4)
    set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /O2")
endif()

# ============================================================================
# Buscar dependencias: Raylib
# ============================================================================
find_package(raylib 5.0 QUIET)

if(NOT raylib_FOUND)
    message(STATUS "Raylib no encontrado en el sistema")
    
    # Determinar la ruta de la librería según la plataforma
    if(WIN32)
        set(RAYLIB_LIB_FILE "${CMAKE_SOURCE_DIR}/vendor/lib/libraylib.a")
    else()
        set(RAYLIB_LIB_FILE "${CMAKE_SOURCE_DIR}/vendor/lib/libraylib.a")
    endif()
    
    # Verificar si existe el archivo de librería (.a)
    if(EXISTS "${RAYLIB_LIB_FILE}" AND EXISTS "${CMAKE_SOURCE_DIR}/vendor/include/raylib.h")
        message(STATUS "Usando Raylib local desde vendor/")
        
        add_library(raylib STATIC IMPORTED)
        set_target_properties(raylib PROPERTIES
            IMPORTED_LOCATION "${RAYLIB_LIB_FILE}"
            INTERFACE_INCLUDE_DIRECTORIES "${CMAKE_SOURCE_DIR}/vendor/include"
        )
        
        if(UNIX AND NOT APPLE)
            target_link_libraries(raylib INTERFACE GL m pthread dl rt X11)
        elseif(WIN32)
            target_link_libraries(raylib INTERFACE winmm gdi32 opengl32)
        endif()
    else()
        message(STATUS "Raylib no encontrado en vendor/, descargando desde GitHub...")
        
        FetchContent_Declare(
            raylib
            GIT_REPOSITORY https://github.com/raysan5/raylib.git
            GIT_TAG 5.5
            GIT_SHALLOW TRUE
        )
        
        # Política de compatibilidad para versiones antiguas de CMake en raylib
        set(CMAKE_POLICY_DEFAULT_CMP0077 NEW)
        
        set(BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
        set(BUILD_GAMES OFF CACHE BOOL "" FORCE)
        
        FetchContent_MakeAvailable(raylib)
        message(STATUS "Raylib descargado y configurado exitosamente")
    endif()
else()
    message(STATUS "Raylib encontrado en el sistema: ${raylib_VERSION}")
endif()

# ============================================================================
# Buscar dependencias: TinyXML2
# ============================================================================
find_package(tinyxml2 QUIET)

if(NOT tinyxml2_FOUND)
    message(STATUS "TinyXML2 no encontrado en el sistema")
    
    set(TINYXML2_LIB_FILE "${CMAKE_SOURCE_DIR}/vendor/lib/libtinyxml2.a")
    
    # Verificar si existe el archivo de librería (.a)
    if(EXISTS "${TINYXML2_LIB_FILE}" AND EXISTS "${CMAKE_SOURCE_DIR}/vendor/include/tinyxml2.h")
        message(STATUS "Usando TinyXML2 local desde vendor/")
        
        add_library(tinyxml2 STATIC IMPORTED)
        set_target_properties(tinyxml2 PROPERTIES
            IMPORTED_LOCATION "${TINYXML2_LIB_FILE}"
            INTERFACE_INCLUDE_DIRECTORIES "${CMAKE_SOURCE_DIR}/vendor/include"
        )
    else()
        message(STATUS "TinyXML2 no encontrado en vendor/, descargando desde GitHub...")
        
        FetchContent_Declare(
            tinyxml2
            GIT_REPOSITORY https://github.com/leethomason/tinyxml2.git
            GIT_TAG 9.0.0
            GIT_SHALLOW TRUE
        )
        
        FetchContent_MakeAvailable(tinyxml2)
        message(STATUS "TinyXML2 descargado y configurado exitosamente")
    endif()
else()
    message(STATUS "TinyXML2 encontrado en el sistema: ${tinyxml2_VERSION}")
endif()

# ============================================================================
# Recopilación de archivos fuente
# ============================================================================
file(GLOB_RECURSE SOURCES "${CMAKE_SOURCE_DIR}/src/*.cpp")

list(LENGTH SOURCES SOURCE_COUNT)
message(STATUS "Archivos fuente encontrados: ${SOURCE_COUNT}")

if(NOT SOURCES)
    message(FATAL_ERROR "No se encontraron archivos .cpp en src/")
endif()

# ============================================================================
# Gestión de Assets
# ============================================================================
if(EXISTS "${CMAKE_SOURCE_DIR}/assets")
    message(STATUS "Copiando assets/ al directorio de build...")
    file(COPY "${CMAKE_SOURCE_DIR}/assets" DESTINATION "${CMAKE_BINARY_DIR}")
endif()

# ============================================================================
# Definición del ejecutable
# ============================================================================
add_executable(chaos-crew ${SOURCES})

# ============================================================================
# Directorios de inclusión
# ============================================================================
target_include_directories(chaos-crew PRIVATE
    ${CMAKE_SOURCE_DIR}/src
    ${CMAKE_SOURCE_DIR}/src/components
    ${CMAKE_SOURCE_DIR}/src/core
    ${CMAKE_SOURCE_DIR}/src/entities
    ${CMAKE_SOURCE_DIR}/src/entt
    ${CMAKE_SOURCE_DIR}/src/states
    ${CMAKE_SOURCE_DIR}/src/systems
)

# ============================================================================
# Enlazado de librerías
# ============================================================================
target_link_libraries(chaos-crew PRIVATE raylib tinyxml2)

# ============================================================================
# Configuración de salida
# ============================================================================
set_target_properties(chaos-crew PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
)

add_custom_command(TARGET chaos-crew POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E make_directory "${CMAKE_SOURCE_DIR}/bin"
    COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:chaos-crew> "${CMAKE_SOURCE_DIR}/bin/"
    COMMENT "Copiando ejecutable a bin/"
)

if(EXISTS "${CMAKE_SOURCE_DIR}/assets")
    add_custom_command(TARGET chaos-crew POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
            "${CMAKE_SOURCE_DIR}/assets"
            "${CMAKE_BINARY_DIR}/assets"
        COMMENT "Actualizando assets en directorio de build"
    )
endif()

# ============================================================================
# Información de build
# ============================================================================
message(STATUS "")
message(STATUS "╔════════════════════════════════════════╗")
message(STATUS "║  Configuración de Chaos Crew           ║")
message(STATUS "╚════════════════════════════════════════╝")
message(STATUS "Versión:           ${PROJECT_VERSION}")
message(STATUS "Compilador:        ${CMAKE_CXX_COMPILER_ID} ${CMAKE_CXX_COMPILER_VERSION}")
message(STATUS "Build Type:        ${CMAKE_BUILD_TYPE}")
message(STATUS "C++ Standard:      ${CMAKE_CXX_STANDARD}")
message(STATUS "Sistema:           ${CMAKE_SYSTEM_NAME}")
message(STATUS "Arquitectura:      ${CMAKE_SYSTEM_PROCESSOR}")
message(STATUS "Archivos fuente:   ${SOURCE_COUNT}")
message(STATUS "")

# ============================================================================
# Reglas de instalación
# ============================================================================
include(GNUInstallDirs)

install(TARGETS chaos-crew
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    COMPONENT Runtime
)

if(EXISTS "${CMAKE_SOURCE_DIR}/assets")
    install(DIRECTORY "${CMAKE_SOURCE_DIR}/assets"
        DESTINATION ${CMAKE_INSTALL_DATAROOTDIR}/chaos-crew
        COMPONENT Assets
        PATTERN "*.md" EXCLUDE
        PATTERN ".git*" EXCLUDE
    )
    message(STATUS "Assets se instalarán en: ${CMAKE_INSTALL_DATAROOTDIR}/chaos-crew/assets")
endif()

if(EXISTS "${CMAKE_SOURCE_DIR}/README.md")
    install(FILES "${CMAKE_SOURCE_DIR}/README.md"
        DESTINATION ${CMAKE_INSTALL_DOCDIR}
        COMPONENT Documentation
    )
endif()

message(STATUS "Prefijo de instalación: ${CMAKE_INSTALL_PREFIX}")