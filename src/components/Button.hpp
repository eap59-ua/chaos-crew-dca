#pragma once
#include <functional>
#include "../entt/entt.hpp"

struct Button {
    int channel;            // ID del grupo (ej: 1)
    bool isPressed;         // Estado actual (pisado o no)
    bool executed;          // Para que la acción ocurra solo una vez al completarse
    
    // La función recibe el registry para poder modificar el mundo
    std::function<void(entt::registry&)> onPressAction; 
};