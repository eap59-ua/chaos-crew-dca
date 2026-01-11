#include <raylib.h>
#include <cmath>
#include "../entt/entt.hpp"
#include "../components/Player.hpp"
#include "../components/Mandos.hpp"
#include "../utils/Logger.hpp"

void InputSystem(entt::registry& registry) {
    auto view = registry.view<Player, Mandos>();

    for (auto entity : view) {
        auto& player = view.get<Player>(entity);
        auto& mando = view.get<Mandos>(entity);

        if (mando.type == InputType::KEYBOARD) {
            // Keyboard input
            player.left = IsKeyDown(mando.left);
            player.right = IsKeyDown(mando.right);
            player.jump = IsKeyPressed(mando.jump);
        }
        else if (mando.type == InputType::GAMEPAD) {
            // Gamepad input
            int gamepad = mando.gamepadIndex;

            if (IsGamepadAvailable(gamepad)) {
                // Left stick horizontal axis
                float axisX = GetGamepadAxisMovement(gamepad, GAMEPAD_AXIS_LEFT_X);

                // Apply deadzone
                if (std::fabs(axisX) < mando.deadzone) {
                    axisX = 0.0f;
                }

                // Convert analog to digital
                player.left = (axisX < -0.3f);
                player.right = (axisX > 0.3f);

                // D-pad support as alternative
                if (IsGamepadButtonDown(gamepad, GAMEPAD_BUTTON_LEFT_FACE_LEFT)) {
                    player.left = true;
                }
                if (IsGamepadButtonDown(gamepad, GAMEPAD_BUTTON_LEFT_FACE_RIGHT)) {
                    player.right = true;
                }

                // Jump button (A/Cross button)
                player.jump = IsGamepadButtonPressed(gamepad, GAMEPAD_BUTTON_RIGHT_FACE_DOWN);
            } else {
                // Gamepad disconnected, reset inputs
                player.left = false;
                player.right = false;
                player.jump = false;
            }
        }
    }
}
