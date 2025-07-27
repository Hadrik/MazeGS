//
// Created by trric on 7/19/2025.
//

#include "MazeGS.h"

#include <imgui.h>

#include "Factory.h"
#include "Logger.h"
#include "Renderer.h"

void MazeGS::loop() {
    Renderer::newFrame();
    ImGui::Begin("Config");

    if (ImGui::BeginCombo("Log target", Logger::get().getTargetName().c_str())) {
        for (const auto& [target, name] : Logger::getTargetOptions()) {
            if (ImGui::Selectable(name.c_str())) {
                _log << "Set logger target: " << name << std::endl;
                Logger::get().setTarget(target);
            }
        }
        ImGui::EndCombo();
    }

    ImGui::Separator();

    if (ImGui::BeginCombo("Generator", _generator ? _generator->getName().c_str() : "None")) {
        for (const auto& name : Factory<IGenerator>::instance().getRegisteredNames()) {
            if (ImGui::Selectable(name.c_str())) {
                _log << "Set generator: " << name << std::endl;
                _generator = std::move(Factory<IGenerator>::instance().create(name));
            }
        }
        ImGui::EndCombo();
    }
    if (_generator) {
        if (ImGui::CollapsingHeader("Generator settings")) {
            _generator->drawGUI();
        }
    }

    ImGui::Separator();

    if (ImGui::BeginCombo("Solver", _solver ? _solver->getName().c_str() : "None")) {
        for (const auto& name : Factory<ISolver>::instance().getRegisteredNames()) {
            if (ImGui::Selectable(name.c_str())) {
                _log << "Set solver: " << name << std::endl;
                _solver = std::move(Factory<ISolver>::instance().create(name));
            }
        }
        ImGui::EndCombo();
    }

    ImGui::End();

    ImGui::Begin("Controls");

    const float w = ImGui::GetWindowWidth() / 2.f - 12;
    ImGui::BeginDisabled(!_generator || _runner.isRunning());
    if (ImGui::Button("Generate", {w, 0})) {
        _log << "Starting generator" << std::endl;
        _runner.run(
            [&](auto& m) { _generator->begin(m); },
            [&]() { return _generator->step(); },
            _maze);
    }
    ImGui::EndDisabled();
    ImGui::SameLine();
    ImGui::BeginDisabled(!_solver || _runner.isRunning());
    if (ImGui::Button("Solve", {w, 0})) {
        _log << "Starting solver" << std::endl;
        _runner.run(
            [&](auto& m) { _solver->begin(m); },
            [&]() { return _solver->step().has_value(); },
            _maze);
    }
    ImGui::EndDisabled();
    _runner.drawGui();

    ImGui::End();
    _renderer.render(_maze.get());
}
