//
// Created by trric on 7/24/2025.
//

#include "StepRunner.h"

#include <imgui.h>

StepRunner::~StepRunner() {
    stop();
}

void StepRunner::run(const BeginFunction& begin, StepFunction step, CleanupFunction clean, std::unique_ptr<Maze> &maze) {
    if (isRunning()) {
        return;
    }
    if (_thread.joinable()) {
        _thread.join();
    }
    begin(maze);
    _stepFunction = std::move(step);
    _cleanupFunction = std::move(clean);
    _isRunning = true;
    _thread = std::thread(&StepRunner::worker, this);
}

void StepRunner::stop() {
    _log << "Stopping runner" << std::endl;
    if (!isRunning()) {
        return;
    }
    _isRunning = false;
    if (_thread.joinable()) {
        _thread.join();
    }
}

void StepRunner::drawGui() {
    if (ImGui::RadioButton("Manual", _mode == Mode::Manual)) {
        _mode = Mode::Manual;
    }
    ImGui::SameLine();
    if (ImGui::RadioButton("Automatic", _mode == Mode::Automatic)) {
        _mode = Mode::Automatic;
    }

    if (_mode == Mode::Manual) {
        if (ImGui::Button("Step")) {
            _stepRequest = true;
        }
    } else {
        ImGui::SliderInt("Delay (ms)", &_delay, 0, 500);
    }

    if (isRunning()) {
        if (ImGui::Button("Stop")) {
            stop();
        }
    }
}

bool StepRunner::isRunning() const {
    return _isRunning;
}

void StepRunner::worker() {
    _log << "Worker started" << std::endl;
    try {
        while (_isRunning) {
            if (_mode == Mode::Automatic) {
                if (_stepFunction()) {
                    _isRunning = false;
                    break;
                }
                std::this_thread::sleep_for(std::chrono::milliseconds(_delay));
            } else {
                if (_stepRequest) {
                    if (_stepFunction()) {
                        _isRunning = false;
                    }
                    _stepRequest = false;
                } else {
                    std::this_thread::sleep_for(std::chrono::milliseconds(10));
                }
            }
        }
        _cleanupFunction();
    } catch (const std::exception& e) {
        _log << "Worker exception: " << e.what() << std::endl;
        _isRunning = false;
    }
    _log << "Worker finished" << std::endl;
}

