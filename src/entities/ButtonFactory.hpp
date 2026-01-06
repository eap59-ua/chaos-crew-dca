#pragma once
#include "../entt/entt.hpp"
#include "raylib.h"

// Crea una entidad botón en la posición (x,y) asignada al canal especificado.
entt::entity createButton(entt::registry& registry, float x, float y, int channel);