.PHONY: all clean info clean-cache dev-deps install uninstall dist run debug help

#
RED    := \033[0;31m
GREEN  := \033[0;32m
YELLOW := \033[1;33m
BLUE   := \033[0;34m
CYAN   := \033[0;36m
NC     := \033[0m # No Color


CXX      := g++
CXXFLAGS := -std=c++17 -Wall -Wextra -Wpedantic
OPTFLAGS := -O3
DBGFLAGS := -g -O0


SRC_DIR    := src
OBJ_DIR    := obj
BIN_DIR    := bin
LIB_DIR    := vendor/lib
INCLUDE    := vendor/include

SRC_SUBDIRS := core entities states systems utils

#Directorios de includes adicionales
INC_DIRS := -I$(SRC_DIR) \
            -I$(SRC_DIR)/components \
            -I$(SRC_DIR)/entt \
            -I$(INCLUDE)

# ============================================================================
# Detección automática de archivos fuente
# ============================================================================
# Encontrar todos los .cpp en los subdirectorios especificados
SOURCES := $(foreach dir,$(SRC_SUBDIRS),$(wildcard $(SRC_DIR)/$(dir)/*.cpp))
# Añadir main.cpp si existe en raíz de src/
SOURCES += $(wildcard $(SRC_DIR)/main.cpp)

# Generar nombres de objetos manteniendo estructura de directorios
OBJECTS := $(patsubst $(SRC_DIR)/%.cpp,$(OBJ_DIR)/%.o,$(SOURCES))

# Archivos de dependencias automáticas
DEPS := $(OBJECTS:.o=.d)

# Nombre del ejecutable
TARGET := $(BIN_DIR)/game


# Raylib y dependencias del sistema
LIBS := -lraylib -lGL -lm -lpthread -ldl -lrt -lX11 -ltinyxml2


# ============================================================================
# Regla por defecto
# ============================================================================
all: banner $(TARGET) success

# ============================================================================
# Banner informativo
# ============================================================================
banner:
	@echo "$(CYAN)╔════════════════════════════════════════╗$(NC)"
	@echo "$(CYAN)║$(NC)  $(GREEN)Compilando Chaos Crew (ECS)$(NC)       $(CYAN)║$(NC)"
	@echo "$(CYAN)╚════════════════════════════════════════╝$(NC)"

success:
	@echo ""
	@echo "$(GREEN)✅ Compilación exitosa!$(NC)"
	@echo "$(YELLOW)📁 Ejecutable: $(TARGET)$(NC)"
	@echo "$(CYAN)▶️  Ejecutar con: make run$(NC)"
	@echo ""

# ============================================================================
# Compilación del ejecutable y  linkeado
# ============================================================================
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	@echo "$(YELLOW)Compilando$(NC) $<"
	@mkdir -p $(dir $@)
	@$(CXX) $(CXXFLAGS) $(OPTFLAGS) $(INC_DIRS) -MMD -MP -c $< -o $@

LDFLAGS := -L$(LIB_DIR)

$(TARGET): $(OBJECTS)
	@echo "$(BLUE)🔗 Linkeando ejecutable...$(NC)"
	@mkdir -p $(BIN_DIR)
	@$(CXX) $(OBJECTS) -o $@ $(LDFLAGS) $(LIBS)


# Incluir archivos de dependencias
-include $(DEPS)

# ============================================================================
# Reglas de limpieza
# ============================================================================
clean:
	@echo "$(RED)🧹 Limpiando archivos de compilación...$(NC)"
	@rm -rf $(OBJ_DIR) $(BIN_DIR)
	@echo "$(GREEN)✅ Limpieza completada$(NC)"

clean-cache:
	@echo "$(RED)🧹 Limpiando caché de ccache...$(NC)"
	@ccache -C
	@ccache -z
	@echo "$(GREEN)✅ Caché limpiada y estadísticas reseteadas$(NC)"

# ============================================================================
# Ejecutar el juego
# ============================================================================
run: all
	@echo "$(GREEN)🎮 Ejecutando Chaos Crew...$(NC)"
	@echo ""
	@./$(TARGET)
# ============================================================================
# REGLAS DE INSTALACIÓN Y DISTRIBUCIÓN
# ============================================================================

PREFIX ?= /usr
DESTDIR ?=
INSTALL_DIR = $(DESTDIR)$(PREFIX)/games/chaos-crew
DESKTOP_DIR = $(DESTDIR)$(PREFIX)/share/applications

.PHONY: install uninstall dist

install: all
	@echo "📦 Instalando Chaos Crew en $(INSTALL_DIR)..."
	@mkdir -p $(INSTALL_DIR)
	@mkdir -p $(DESKTOP_DIR)
	@cp $(TARGET) $(INSTALL_DIR)/
	@cp -r assets $(INSTALL_DIR)/
	@cp chaos-crew.desktop $(DESKTOP_DIR)/
	@chmod +x $(INSTALL_DIR)/game
	@echo "✅ Instalación completada"

uninstall:
	@echo "🗑️  Desinstalando Chaos Crew..."
	@rm -rf $(INSTALL_DIR)
	@rm -f $(DESKTOP_DIR)/chaos-crew.desktop
	@echo "✅ Desinstalación completada"

dist: clean
	@echo "📦 Creando paquete .deb..."
	@dpkg-buildpackage -us -uc -b 
#echo "✅ Paquete creado: chaos-crew_0.1.0-alpha_amd64.deb"
# ============================================================================
# Información del proyecto
# ============================================================================
info:
	@echo "$(YELLOW)Compilador:$(NC)     $(CXX)"
	@echo "$(YELLOW)Flags:$(NC)          $(CXXFLAGS) $(OPTFLAGS)"
	@echo "$(YELLOW)Archivos .cpp:$(NC)  $(words $(SOURCES))"
	@echo "$(YELLOW)Subdirectorios:$(NC) $(SRC_SUBDIRS)"
	@echo "$(YELLOW)Ejecutable:$(NC)     $(TARGET)"
	@echo "$(YELLOW)Librerías:$(NC)      Raylib + EnTT (header-only)"
	@echo "$(CYAN)═══════════════════════════════════════════════$(NC)"
	@echo ""
	@echo "$(BLUE) Estadísticas ccache:$(NC)"
	@ccache -s | grep -E "(cache (hit|miss)|files in cache|cache size)" || echo "  ccache no disponible"
	@echo ""

# ============================================================================
# Listar archivos fuente detectados
# ============================================================================
list-sources:
	@echo "$(BLUE) Archivos fuente detectados:$(NC)"
	@for src in $(SOURCES); do \
		echo "  - $$src"; \
	done
	@echo ""
	@echo "$(YELLOW)Total: $(words $(SOURCES)) archivos$(NC)"

# ============================================================================
# REGLAS DE INSTALACIÓN Y DISTRIBUCIÓN
# ============================================================================


	

# ============================================================================
# Información adicional
# ============================================================================
.DEFAULT_GOAL := all