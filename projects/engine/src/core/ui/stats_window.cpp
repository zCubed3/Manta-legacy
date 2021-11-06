#include "stats_window.hpp"

#include <imgui/imgui.h>

#include <core/engine.hpp>

void StatsWindow::DrawGUI(MEngine *engine) {
    if (!open)
        return;

    ImGui::Begin("Stats");

    ImGui::Text("TODO");

    int rawFPS = roundf(1.0f / engine->world.timeDelta);

    ImGui::Text("FPS = %i", rawFPS);

    ImGui::End();
}