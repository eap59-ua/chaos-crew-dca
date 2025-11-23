#pragma once
#include "../entt/entt.hpp"
#include <raylib.h>
#include "../components/PlayerAnimations.hpp" // Necesario para la estructura de animaciones

entt::entity createPlayer(entt::registry& registry, float x, float y, PlayerAnimations anims, KeyboardKey LEFT, KeyboardKey RIGHT, KeyboardKey JUMP);