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

    # En Windows (especialmente con MinGW) es común que una librería precompilada
    # no sea compatible con el toolchain del usuario. Para evitar errores de link
    # del tipo "undefined reference to InitWindow/LoadTexture/...", forzamos a
    # compilar Raylib desde fuentes.
    if(WIN32)
        message(STATUS "Windows detectado: compilando Raylib desde GitHub para asegurar compatibilidad")

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
    else()
    
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
# Buscar dependencias: spdlog (Logging System)
# ============================================================================
message(STATUS "Configurando spdlog...")

FetchContent_Declare(
    spdlog
    GIT_REPOSITORY https://github.com/gabime/spdlog.git
    GIT_TAG v1.12.0
    GIT_SHALLOW TRUE
)

FetchContent_MakeAvailable(spdlog)
message(STATUS "spdlog descargado y configurado exitosamente")

# ============================================================================
# Buscar dependencias: Dear ImGui y rlImGui (Debug Interface)
# ============================================================================
message(STATUS "Configurando Dear ImGui...")

# First, download Dear ImGui
FetchContent_Declare(
    imgui
    GIT_REPOSITORY https://github.com/ocornut/imgui.git
    GIT_TAG v1.92.1
    GIT_SHALLOW TRUE
)

FetchContent_GetProperties(imgui)
if(NOT imgui_POPULATED)
    FetchContent_Populate(imgui)
endif()

message(STATUS "Dear ImGui v1.92.1 descargado exitosamente")
message(STATUS "Configurando rlImGui...")

# Then, download rlImGui
FetchContent_Declare(
    rlImGui
    GIT_REPOSITORY https://github.com/raylib-extras/rlImGui.git
    GIT_TAG main
    GIT_SHALLOW TRUE
)

FetchContent_GetProperties(rlImGui)
if(NOT rlimgui_POPULATED)
    FetchContent_Populate(rlImGui)
    # rlImGui doesn't have CMakeLists, so we add it manually
    # Combine both Dear ImGui and rlImGui into a single library
    add_library(rlImGui STATIC
        # Dear ImGui core files
        ${imgui_SOURCE_DIR}/imgui.cpp
        ${imgui_SOURCE_DIR}/imgui_draw.cpp
        ${imgui_SOURCE_DIR}/imgui_tables.cpp
        ${imgui_SOURCE_DIR}/imgui_widgets.cpp
        # rlImGui integration
        ${rlimgui_SOURCE_DIR}/rlImGui.cpp
    )
    target_include_directories(rlImGui PUBLIC
        ${imgui_SOURCE_DIR}
        ${rlimgui_SOURCE_DIR}
    )
    target_link_libraries(rlImGui PUBLIC raylib)
endif()

message(STATUS "rlImGui configurado exitosamente")

# ============================================================================
# Buscar dependencias: GNU Gettext (internacionalización)
# ============================================================================
find_package(Gettext)

# FindGettext solo localiza las herramientas (msgfmt/msgmerge), pero nuestro código
# también necesita el header y la librería de libintl para usar gettext en runtime.
include(CheckIncludeFileCXX)
check_include_file_cxx("libintl.h" CHAOSCREW_HAVE_LIBINTL_H)

if(CHAOSCREW_HAVE_LIBINTL_H)
    # En MinGW/MSYS2 suele ser libintl; en Linux puede ser opcional.
    find_library(CHAOSCREW_INTL_LIB intl)
endif()

set(CHAOSCREW_ENABLE_GETTEXT 0)

if(GETTEXT_FOUND AND CHAOSCREW_HAVE_LIBINTL_H)
    # Si no se encuentra la librería, igualmente permitimos compilar sin gettext
    # (se usará el fallback sin i18n). Solo habilitamos i18n si existe libintl.
    if(CHAOSCREW_INTL_LIB)
        set(CHAOSCREW_ENABLE_GETTEXT 1)
    endif()
endif()

if(CHAOSCREW_ENABLE_GETTEXT)
    message(STATUS "Gettext encontrado: ${GETTEXT_MSGFMT_EXECUTABLE}")
    message(STATUS "libintl encontrado: ${CHAOSCREW_INTL_LIB}")

    # Compilar archivos .po a .mo para español
    add_custom_command(
        OUTPUT "${CMAKE_BINARY_DIR}/po/es_ES/LC_MESSAGES/chaos-crew.mo"
        COMMAND ${CMAKE_COMMAND} -E make_directory "${CMAKE_BINARY_DIR}/po/es_ES/LC_MESSAGES"
        COMMAND ${GETTEXT_MSGFMT_EXECUTABLE} -o "${CMAKE_BINARY_DIR}/po/es_ES/LC_MESSAGES/chaos-crew.mo"
                "${CMAKE_SOURCE_DIR}/po/es_ES/LC_MESSAGES/chaos-crew.po"
        DEPENDS "${CMAKE_SOURCE_DIR}/po/es_ES/LC_MESSAGES/chaos-crew.po"
        COMMENT "Compilando traducción español (es_ES)"
    )

    # Compilar archivos .po a .mo para inglés
    add_custom_command(
        OUTPUT "${CMAKE_BINARY_DIR}/po/en_US/LC_MESSAGES/chaos-crew.mo"
        COMMAND ${CMAKE_COMMAND} -E make_directory "${CMAKE_BINARY_DIR}/po/en_US/LC_MESSAGES"
        COMMAND ${GETTEXT_MSGFMT_EXECUTABLE} -o "${CMAKE_BINARY_DIR}/po/en_US/LC_MESSAGES/chaos-crew.mo"
                "${CMAKE_SOURCE_DIR}/po/en_US/LC_MESSAGES/chaos-crew.po"
        DEPENDS "${CMAKE_SOURCE_DIR}/po/en_US/LC_MESSAGES/chaos-crew.po"
        COMMENT "Compilando traducción inglés (en_US)"
    )

    # Crear target custom para las traducciones
    add_custom_target(translations ALL
        DEPENDS
            "${CMAKE_BINARY_DIR}/po/es_ES/LC_MESSAGES/chaos-crew.mo"
            "${CMAKE_BINARY_DIR}/po/en_US/LC_MESSAGES/chaos-crew.mo"
    )

    message(STATUS "Internacionalización habilitada (es_ES, en_US)")
else()
    message(WARNING "Gettext/libintl no encontrado - internacionalización deshabilitada")
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
# Gestión de Mapas
# ============================================================================
if(EXISTS "${CMAKE_SOURCE_DIR}/mapas")
    message(STATUS "Copiando mapas/ al directorio de build...")
    file(COPY "${CMAKE_SOURCE_DIR}/mapas" DESTINATION "${CMAKE_BINARY_DIR}")
endif()

# ============================================================================
# Definición del ejecutable
# ============================================================================
add_executable(chaos-crew ${SOURCES})

# Habilitar/deshabilitar gettext en el código.
target_compile_definitions(chaos-crew PRIVATE CHAOSCREW_ENABLE_GETTEXT=${CHAOSCREW_ENABLE_GETTEXT})

# ============================================================================
# Directorios de inclusión
# ============================================================================
target_include_directories(chaos-crew PRIVATE
    ${CMAKE_SOURCE_DIR}/src
    ${CMAKE_SOURCE_DIR}/src/components
    ${CMAKE_SOURCE_DIR}/src/core
    ${CMAKE_SOURCE_DIR}/src/debug
    ${CMAKE_SOURCE_DIR}/src/entities
    ${CMAKE_SOURCE_DIR}/src/entt
    ${CMAKE_SOURCE_DIR}/src/locale
    ${CMAKE_SOURCE_DIR}/src/states
    ${CMAKE_SOURCE_DIR}/src/systems
    ${CMAKE_SOURCE_DIR}/src/utils
)

# ============================================================================
# Enlazado de librerías
# ============================================================================
# Orden importante con librerías estáticas en MinGW: dependientes primero.
target_link_libraries(chaos-crew PRIVATE rlImGui raylib tinyxml2 spdlog::spdlog)

# Enlazar libintl únicamente si está disponible y la i18n está habilitada.
if(CHAOSCREW_ENABLE_GETTEXT)
    target_link_libraries(chaos-crew PRIVATE ${CHAOSCREW_INTL_LIB})
endif()

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
        COMMAND ${CMAKE_COMMAND} -E copy_directory
            "${CMAKE_SOURCE_DIR}/assets"
            "${CMAKE_BINARY_DIR}/bin/assets"
        COMMENT "Actualizando assets en directorio de build"
    )
endif()

# Copiar mapas al directorio de ejecución
if(EXISTS "${CMAKE_SOURCE_DIR}/mapas")
    add_custom_command(TARGET chaos-crew POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
            "${CMAKE_SOURCE_DIR}/mapas"
            "${CMAKE_BINARY_DIR}/bin/mapas"
        COMMENT "Copiando mapas al directorio de ejecución"
    )
endif()

# Copiar archivos de traducción al directorio de build
if(CHAOSCREW_ENABLE_GETTEXT)
    add_custom_command(TARGET chaos-crew POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
            "${CMAKE_BINARY_DIR}/po"
            "${CMAKE_BINARY_DIR}/bin/po"
        COMMENT "Copiando traducciones al directorio de ejecución"
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

# Instalar mapas (niveles del juego) - CRÍTICO para que el juego funcione
if(EXISTS "${CMAKE_SOURCE_DIR}/mapas")
    install(DIRECTORY "${CMAKE_SOURCE_DIR}/mapas"
        DESTINATION ${CMAKE_INSTALL_DATAROOTDIR}/chaos-crew
        COMPONENT Assets
        FILES_MATCHING
        PATTERN "*.xml"
        PATTERN "*.tmx"   # Por si se usa Tiled
        PATTERN "*.json"  # Por si se usan otros formatos
    )
    message(STATUS "Mapas se instalarán en: ${CMAKE_INSTALL_DATAROOTDIR}/chaos-crew/mapas")
endif()

# Documentación
if(EXISTS "${CMAKE_SOURCE_DIR}/README.md")
    install(FILES
        "${CMAKE_SOURCE_DIR}/README.md"
        DESTINATION ${CMAKE_INSTALL_DOCDIR}
        COMPONENT Documentation
    )
endif()

if(EXISTS "${CMAKE_SOURCE_DIR}/docs/HITO3.md")
    install(FILES
        "${CMAKE_SOURCE_DIR}/docs/HITO3.md"
        DESTINATION ${CMAKE_INSTALL_DOCDIR}
        COMPONENT Documentation
    )
endif()

# Instalar traducciones (.mo) - SIEMPRE, sin depender de GETTEXT_FOUND
# Los archivos .mo ya deben estar compilados en build/po/
install(FILES
    "${CMAKE_BINARY_DIR}/po/es_ES/LC_MESSAGES/chaos-crew.mo"
    DESTINATION ${CMAKE_INSTALL_DATAROOTDIR}/locale/es_ES/LC_MESSAGES
    COMPONENT Translations
    OPTIONAL  # No fallar si no existe (para builds sin gettext)
)

install(FILES
    "${CMAKE_BINARY_DIR}/po/en_US/LC_MESSAGES/chaos-crew.mo"
    DESTINATION ${CMAKE_INSTALL_DATAROOTDIR}/locale/en_US/LC_MESSAGES
    COMPONENT Translations
    OPTIONAL
)

message(STATUS "Traducciones se instalarán en: ${CMAKE_INSTALL_DATAROOTDIR}/locale")

message(STATUS "Prefijo de instalación: ${CMAKE_INSTALL_PREFIX}")

# ============================================================================
# CPACK CONFIGURATION
# ============================================================================

# Información del paquete
set(CPACK_PACKAGE_NAME "chaos-crew")
set(CPACK_PACKAGE_VERSION "${PROJECT_VERSION}")
set(CPACK_PACKAGE_VENDOR "Chaos Crew Team - Universidad de Alicante")
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "Cooperative 2D platformer game")
set(CPACK_PACKAGE_DESCRIPTION "Chaos Crew is a cooperative 2D platformer game where 2-5 players must work together to overcome levels full of dynamic traps and puzzles.")
set(CPACK_PACKAGE_CONTACT "eap59@alu.ua.es")

# Archivos de licencia y README
if(EXISTS "${CMAKE_SOURCE_DIR}/LICENSE")
    set(CPACK_RESOURCE_FILE_LICENSE "${CMAKE_SOURCE_DIR}/LICENSE")
endif()
if(EXISTS "${CMAKE_SOURCE_DIR}/README.md")
    set(CPACK_RESOURCE_FILE_README "${CMAKE_SOURCE_DIR}/README.md")
endif()

# Directorio de salida de paquetes
set(CPACK_PACKAGE_DIRECTORY "${CMAKE_BINARY_DIR}/packages")

# ============================================================================
# CONFIGURACIÓN ESPECÍFICA DE DEB (Debian/Ubuntu)
# ============================================================================
set(CPACK_DEBIAN_PACKAGE_MAINTAINER "Chaos Crew Team <eap59@alu.ua.es>")
set(CPACK_DEBIAN_PACKAGE_SECTION "games")
set(CPACK_DEBIAN_PACKAGE_PRIORITY "optional")
set(CPACK_DEBIAN_PACKAGE_HOMEPAGE "https://github.com/eap59-ua/chaos-crew-dca")

# Dependencias del paquete DEB
# CRÍTICO: gettext-runtime debe estar instalado para que funcione i18n
set(CPACK_DEBIAN_PACKAGE_DEPENDS "libc6 (>= 2.31), libstdc++6 (>= 10), libtinyxml2-9")
set(CPACK_DEBIAN_PACKAGE_RECOMMENDS "gettext")

# Arquitectura
set(CPACK_DEBIAN_PACKAGE_ARCHITECTURE "amd64")

# Nombre del archivo DEB
set(CPACK_DEBIAN_FILE_NAME "DEB-DEFAULT")

# ============================================================================
# CONFIGURACIÓN ESPECÍFICA DE ZIP (Windows/Generic)
# ============================================================================
set(CPACK_GENERATOR "DEB;ZIP")

# Nombre del archivo ZIP
set(CPACK_ARCHIVE_FILE_NAME "${CPACK_PACKAGE_NAME}-${CPACK_PACKAGE_VERSION}-${CMAKE_SYSTEM_NAME}")

# Para Windows: incluir todo lo necesario en el ZIP
if(WIN32)
    # Incluir DLLs necesarias (si las hay)
    install(DIRECTORY "${CMAKE_BINARY_DIR}/bin/"
        DESTINATION bin
        OPTIONAL
        FILES_MATCHING
        PATTERN "*.dll"
    )
endif()

# Componentes a incluir en el paquete
set(CPACK_COMPONENTS_ALL Runtime Assets Translations Documentation)

# Descripción de componentes
set(CPACK_COMPONENT_RUNTIME_DISPLAY_NAME "Game Executable")
set(CPACK_COMPONENT_RUNTIME_DESCRIPTION "Chaos Crew game executable")
set(CPACK_COMPONENT_RUNTIME_REQUIRED TRUE)

set(CPACK_COMPONENT_ASSETS_DISPLAY_NAME "Game Assets")
set(CPACK_COMPONENT_ASSETS_DESCRIPTION "Graphics, sounds, maps and other game resources")
set(CPACK_COMPONENT_ASSETS_REQUIRED TRUE)

set(CPACK_COMPONENT_TRANSLATIONS_DISPLAY_NAME "Translations")
set(CPACK_COMPONENT_TRANSLATIONS_DESCRIPTION "Language files (Spanish, English)")
set(CPACK_COMPONENT_TRANSLATIONS_REQUIRED FALSE)

set(CPACK_COMPONENT_DOCUMENTATION_DISPLAY_NAME "Documentation")
set(CPACK_COMPONENT_DOCUMENTATION_DESCRIPTION "README and documentation files")
set(CPACK_COMPONENT_DOCUMENTATION_REQUIRED FALSE)

# Incluir CPack
include(CPack)

message(STATUS "")
message(STATUS "CPack configurado para generar paquetes DEB y ZIP")
message(STATUS "Ejecutar 'cpack' en el directorio de build para generar paquetes")
