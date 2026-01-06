#pragma once
#include <functional>
#include "raylib.h"        // Necesario para Rectangle y Color
#include "../entt/entt.hpp"

struct Button {
    int channel;            // ID del grupo (ej: 1)
    bool isPressed;         // Estado actual (pisado o no)
    bool executed;          // Para que la acción ocurra solo una vez
    
    // La función recibe el registry para poder modificar el mundo
    std::function<void(entt::registry&)> onPressAction; 

    // --- NUEVOS CAMPOS VISUALES ---
    Rectangle bounds;       // Posición y tamaño visual (x, y, w, h)
    Color color;            // Color actual del botón
};