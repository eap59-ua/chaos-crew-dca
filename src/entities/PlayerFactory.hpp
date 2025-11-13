#pragma once
#include "../entt/entt.hpp"
#include <raylib.h>

entt::entity createPlayer(entt::registry& registry, float x, float y, Color color, KeyboardKey LEFT, KeyboardKey RIGHT, KeyboardKey JUMP);