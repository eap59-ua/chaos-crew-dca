#pragma once
#include <raylib.h>

enum class InputType {
    KEYBOARD,
    GAMEPAD
};

struct Mandos {
    // Input type (keyboard or gamepad)
    InputType type = InputType::KEYBOARD;

    // Keyboard controls (used when type == KEYBOARD)
    KeyboardKey left = KEY_NULL;
    KeyboardKey right = KEY_NULL;
    KeyboardKey jump = KEY_NULL;

    // Gamepad index (0-3, used when type == GAMEPAD)
    int gamepadIndex = 0;

    // Gamepad axis deadzone
    float deadzone = 0.15f;
};