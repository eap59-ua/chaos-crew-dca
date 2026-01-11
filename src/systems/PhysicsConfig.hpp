#pragma once

/**
 * @brief Global physics configuration for player movement
 *
 * These variables are mutable to allow real-time tweaking via ImGui debug UI.
 * Access globally via PhysicsConfig::Instance()
 */
struct PhysicsConfig {
    // Horizontal movement
    float H_SPEED = 200.0f;          // Horizontal speed (px/s)
    float AIR_CONTROL = 0.85f;       // Air control factor (0-1)

    // Vertical movement
    float GRAVITY = 900.0f;          // Gravity (px/s²)
    float JUMP_FORCE = -600.0f;      // Jump force (negative = upward)
    float MAX_FALL_SPEED = 1200.0f;  // Maximum falling speed (px/s)

    /**
     * @brief Get the singleton instance of PhysicsConfig
     * @return Reference to the physics configuration
     */
    static PhysicsConfig& Instance() {
        static PhysicsConfig instance;
        return instance;
    }

private:
    PhysicsConfig() = default;
};
