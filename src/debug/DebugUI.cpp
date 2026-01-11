#include "DebugUI.hpp"
#include "utils/Logger.hpp"
#include <raylib.h>

DebugUI& DebugUI::GetInstance() {
    static DebugUI instance;
    return instance;
}

void DebugUI::Init() {
    rlImGuiSetup(true); // true = dark theme
    LOG_INFO("[DebugUI] ImGui initialized with dark theme");
}

void DebugUI::Shutdown() {
    rlImGuiShutdown();
    LOG_INFO("[DebugUI] ImGui shutdown");
}

void DebugUI::NewFrame() {
    if (!enabled_) return;
    rlImGuiBegin();
}

void DebugUI::Render() {
    if (!enabled_) return;

    DrawMainWindow();

    rlImGuiEnd();
}

void DebugUI::DrawMainWindow() {
    // Set window position and size on first use
    ImGui::SetNextWindowPos(ImVec2(10, 10), ImGuiCond_FirstUseEver);
    ImGui::SetNextWindowSize(ImVec2(420, 650), ImGuiCond_FirstUseEver);

    // Main debug window
    if (ImGui::Begin("Chaos Crew - Debug Panel", &enabled_, ImGuiWindowFlags_AlwaysAutoResize)) {

        // === SYSTEM INFO ===
        ImGui::SeparatorText("System Info");
        ImGui::Text("FPS: %d", GetFPS());
        ImGui::Text("Frame Time: %.2f ms", GetFrameTime() * 1000.0f);
        ImGui::Spacing();

        // === FLOAT PARAMETERS ===
        if (!floats_.empty()) {
            if (ImGui::CollapsingHeader("Float Parameters", ImGuiTreeNodeFlags_DefaultOpen)) {
                for (auto& var : floats_) {
                    ImGui::SliderFloat(var.name.c_str(), var.ptr, var.min, var.max);
                }
                ImGui::Spacing();
            }
        }

        // === INTEGER PARAMETERS ===
        if (!ints_.empty()) {
            if (ImGui::CollapsingHeader("Integer Parameters", ImGuiTreeNodeFlags_DefaultOpen)) {
                for (auto& var : ints_) {
                    ImGui::SliderInt(var.name.c_str(), var.ptr, var.min, var.max);
                }
                ImGui::Spacing();
            }
        }

        // === BOOLEAN TOGGLES ===
        if (!bools_.empty()) {
            if (ImGui::CollapsingHeader("Boolean Toggles", ImGuiTreeNodeFlags_DefaultOpen)) {
                for (auto& var : bools_) {
                    ImGui::Checkbox(var.name.c_str(), var.ptr);
                }
                ImGui::Spacing();
            }
        }

        // === ACTION BUTTONS ===
        if (!buttons_.empty()) {
            if (ImGui::CollapsingHeader("Actions", ImGuiTreeNodeFlags_DefaultOpen)) {
                for (auto& btn : buttons_) {
                    if (ImGui::Button(btn.name.c_str(), ImVec2(200, 0))) {
                        btn.callback();
                        LOG_DEBUG("[DebugUI] Button pressed: {}", btn.name);
                    }
                }
                ImGui::Spacing();
            }
        }

        // === HELP ===
        ImGui::SeparatorText("Controls");
        ImGui::TextDisabled("F1: Toggle this window");
        ImGui::TextDisabled("ESC: Exit game");
    }
    ImGui::End();
}

void DebugUI::RegisterFloat(const std::string& name, float* value, float min, float max) {
    floats_.push_back({name, value, min, max});
    LOG_DEBUG("[DebugUI] Registered float: {} (range: {} to {})", name, min, max);
}

void DebugUI::RegisterInt(const std::string& name, int* value, int min, int max) {
    ints_.push_back({name, value, min, max});
    LOG_DEBUG("[DebugUI] Registered int: {} (range: {} to {})", name, min, max);
}

void DebugUI::RegisterBool(const std::string& name, bool* value) {
    bools_.push_back({name, value});
    LOG_DEBUG("[DebugUI] Registered bool: {}", name);
}

void DebugUI::RegisterButton(const std::string& name, std::function<void()> callback) {
    buttons_.push_back({name, callback});
    LOG_DEBUG("[DebugUI] Registered button: {}", name);
}

void DebugUI::ClearAll() {
    floats_.clear();
    ints_.clear();
    bools_.clear();
    buttons_.clear();
    LOG_INFO("[DebugUI] Cleared all registered variables");
}
