#pragma once

#include "rlImGui.h"
#include "imgui.h"
#include <functional>
#include <vector>
#include <string>

/**
 * @brief Singleton DebugUI class for ImGui integration with Raylib
 *
 * Provides real-time debugging interface for tweaking gameplay parameters,
 * monitoring system state, and triggering debug actions.
 *
 * Usage:
 *   DebugUI::GetInstance().Init();
 *   DebugUI::GetInstance().RegisterFloat("Speed", &speed, 0.0f, 500.0f);
 *
 *   // In game loop:
 *   DebugUI::GetInstance().NewFrame();  // Before rendering
 *   DebugUI::GetInstance().Render();    // After rendering
 */
class DebugUI {
public:
    /**
     * @brief Get the singleton instance
     * @return Reference to the DebugUI instance
     */
    static DebugUI& GetInstance();

    /**
     * @brief Initialize rlImGui (call after InitWindow)
     */
    void Init();

    /**
     * @brief Shutdown rlImGui (call before CloseWindow)
     */
    void Shutdown();

    /**
     * @brief Begin ImGui frame (call at start of game loop)
     */
    void NewFrame();

    /**
     * @brief Render ImGui (call after game rendering, before EndDrawing)
     */
    void Render();

    /**
     * @brief Check if debug UI is currently visible
     * @return True if enabled, false otherwise
     */
    bool IsEnabled() const { return enabled_; }

    /**
     * @brief Toggle debug UI visibility
     */
    void Toggle() { enabled_ = !enabled_; }

    /**
     * @brief Enable debug UI
     */
    void Enable() { enabled_ = true; }

    /**
     * @brief Disable debug UI
     */
    void Disable() { enabled_ = false; }

    /**
     * @brief Register a float variable with slider control
     * @param name Display name in UI
     * @param value Pointer to the float variable
     * @param min Minimum value for slider
     * @param max Maximum value for slider
     */
    void RegisterFloat(const std::string& name, float* value, float min, float max);

    /**
     * @brief Register an integer variable with slider control
     * @param name Display name in UI
     * @param value Pointer to the int variable
     * @param min Minimum value for slider
     * @param max Maximum value for slider
     */
    void RegisterInt(const std::string& name, int* value, int min, int max);

    /**
     * @brief Register a boolean variable with checkbox control
     * @param name Display name in UI
     * @param value Pointer to the bool variable
     */
    void RegisterBool(const std::string& name, bool* value);

    /**
     * @brief Register a button with callback function
     * @param name Button label
     * @param callback Function to call when button is pressed
     */
    void RegisterButton(const std::string& name, std::function<void()> callback);

    /**
     * @brief Clear all registered variables and buttons
     */
    void ClearAll();

private:
    DebugUI() = default;
    ~DebugUI() = default;

    // Prevent copying
    DebugUI(const DebugUI&) = delete;
    DebugUI& operator=(const DebugUI&) = delete;

    bool enabled_ = true;

    // Variable registration structures
    struct FloatVar {
        std::string name;
        float* ptr;
        float min;
        float max;
    };

    struct IntVar {
        std::string name;
        int* ptr;
        int min;
        int max;
    };

    struct BoolVar {
        std::string name;
        bool* ptr;
    };

    struct ButtonVar {
        std::string name;
        std::function<void()> callback;
    };

    // Storage for registered variables
    std::vector<FloatVar> floats_;
    std::vector<IntVar> ints_;
    std::vector<BoolVar> bools_;
    std::vector<ButtonVar> buttons_;

    /**
     * @brief Draw the main debug window with all controls
     */
    void DrawMainWindow();
};
