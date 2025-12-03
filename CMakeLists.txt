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
# Opciones de compilación
# ============================================================================
# Flags de compilación según el compilador y plataforma
if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
    add_compile_options(-Wall -Wextra -Wpedantic)

    # Optimización para Release
    set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O3")

    # Debug con información completa
    set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -g -O0")
elseif(MSVC)
    add_compile_options(/W4)

    # Optimización para Release en MSVC
    set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /O2")
endif()

# ============================================================================
# Buscar dependencias: Raylib
# ============================================================================
# Buscar Raylib usando find_package
# Primero intenta encontrar Raylib instalado en el sistema
find_package(raylib 5.0 QUIET)

if(NOT raylib_FOUND)
    message(STATUS "Raylib no encontrado en el sistema, buscando en vendor/")

    # Si no se encuentra, usar la versión local en vendor/
    set(RAYLIB_INCLUDE_DIR "${CMAKE_SOURCE_DIR}/vendor/include")
    set(RAYLIB_LIBRARY_DIR "${CMAKE_SOURCE_DIR}/vendor/lib")

    if(EXISTS "${RAYLIB_INCLUDE_DIR}/raylib.h")
        message(STATUS "Usando Raylib local en vendor/")

        # Crear target importado para Raylib local
        add_library(raylib STATIC IMPORTED)
        set_target_properties(raylib PROPERTIES
            IMPORTED_LOCATION "${RAYLIB_LIBRARY_DIR}/libraylib.a"
            INTERFACE_INCLUDE_DIRECTORIES "${RAYLIB_INCLUDE_DIR}"
        )

        # Definir dependencias de sistema para Raylib según la plataforma
        if(UNIX AND NOT APPLE)
            # Linux
            target_link_libraries(raylib INTERFACE
                GL m pthread dl rt X11
            )
        elseif(WIN32)
            # Windows
            target_link_libraries(raylib INTERFACE
                winmm gdi32 opengl32
            )
        endif()
    else()
        message(FATAL_ERROR "Raylib no encontrado. Instálalo o colócalo en vendor/")
    endif()
else()
    message(STATUS "Raylib encontrado: ${raylib_VERSION}")
endif()

# ============================================================================
# Recopilación de archivos fuente
# ============================================================================
# Buscar recursivamente todos los archivos .cpp en src/
file(GLOB_RECURSE SOURCES
    "${CMAKE_SOURCE_DIR}/src/*.cpp"
)

# Mostrar información sobre los archivos encontrados
list(LENGTH SOURCES SOURCE_COUNT)
message(STATUS "Archivos fuente encontrados: ${SOURCE_COUNT}")

# Verificar que se encontraron archivos fuente
if(NOT SOURCES)
    message(FATAL_ERROR "No se encontraron archivos .cpp en src/")
endif()

# ============================================================================
# Gestión de Assets
# ============================================================================
# Copiar assets durante la configuración de CMake
if(EXISTS "${CMAKE_SOURCE_DIR}/assets")
    message(STATUS "Copiando assets/ al directorio de build...")
    file(COPY "${CMAKE_SOURCE_DIR}/assets"
        DESTINATION "${CMAKE_BINARY_DIR}"
    )
endif()

# ============================================================================
# Definición del ejecutable
# ============================================================================
add_executable(chaos-crew ${SOURCES})

# ============================================================================
# Directorios de inclusión
# ============================================================================
# Añadir src/ y sus subdirectorios al include path
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
# Enlazar con Raylib
target_link_libraries(chaos-crew PRIVATE raylib)

# ============================================================================
# Configuración de salida
# ============================================================================
# Directorio de salida para el ejecutable
set_target_properties(chaos-crew PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
)

# Copiar el ejecutable a bin/ en la raíz del proyecto (opcional)
add_custom_command(TARGET chaos-crew POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E make_directory "${CMAKE_SOURCE_DIR}/bin"
    COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:chaos-crew> "${CMAKE_SOURCE_DIR}/bin/"
    COMMENT "Copiando ejecutable a bin/"
)

# Copiar assets durante POST_BUILD (para tenerlos actualizados)
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
# Incluir soporte para GNUInstallDirs (define CMAKE_INSTALL_BINDIR, etc.)
include(GNUInstallDirs)

# Instalar el ejecutable en bin/
install(TARGETS chaos-crew
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    COMPONENT Runtime
)

# Instalar assets en share/chaos-crew/assets/
if(EXISTS "${CMAKE_SOURCE_DIR}/assets")
    install(DIRECTORY "${CMAKE_SOURCE_DIR}/assets"
        DESTINATION ${CMAKE_INSTALL_DATAROOTDIR}/chaos-crew
        COMPONENT Assets
        PATTERN "*.md" EXCLUDE        # Excluir archivos markdown si existen
        PATTERN ".git*" EXCLUDE        # Excluir archivos git
    )
    message(STATUS "Assets se instalarán en: ${CMAKE_INSTALL_DATAROOTDIR}/chaos-crew/assets")
endif()

# Instalar documentación (opcional)
if(EXISTS "${CMAKE_SOURCE_DIR}/README.md")
    install(FILES "${CMAKE_SOURCE_DIR}/README.md"
        DESTINATION ${CMAKE_INSTALL_DOCDIR}
        COMPONENT Documentation
    )
endif()

message(STATUS "Prefijo de instalación: ${CMAKE_INSTALL_PREFIX}")
