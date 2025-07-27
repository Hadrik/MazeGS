//
// Created by trric on 7/20/2025.
//

#include "Logger.h"

#include <imgui.h>

Logger& Logger::get() {
    static Logger _logger;
    return _logger;
}


Logger& Logger::operator<<(std::ostream& (*manip)(std::ostream&)) {
    if (_target != Target::None) {
        manip(_buffer);
        if (_target == Target::Cout) {
            manip(std::cout);
        }
        if (_target == Target::Window) {
            flushBuffer();
        }
    }
    return *this;
}

void Logger::setTarget(const Target target) {
    _target = target;
    if (_target == Target::Window) {
        flushBuffer();
    }
}

std::string Logger::getTargetName() const {
    return getTargetName(_target);
}

std::string Logger::getTargetName(const Target t) {
    switch (t) {
        case Target::None:
            return "None";
        case Target::Cout:
            return "Console";
        case Target::Window:
            return "Window";
    }
    return "???";
}

std::vector<std::tuple<Logger::Target, std::string>> Logger::getTargetOptions() {
    std::vector<std::tuple<Logger::Target, std::string>> options;
    options.emplace_back(Target::None, getTargetName(Target::None));
    options.emplace_back(Target::Cout, getTargetName(Target::Cout));
    options.emplace_back(Target::Window, getTargetName(Target::Window));
    return options;
}

void Logger::drawWindow() {
    if (_target != Target::Window) return;

    if (ImGui::Begin("Logger")) {
        if (ImGui::Button("Clear")) {
            clear();
        }

        ImGui::Separator();

        if (ImGui::BeginChild("LogText", ImVec2(0, 0), false, ImGuiWindowFlags_HorizontalScrollbar)) {
            for (const auto& line : _lines) {
                ImGui::TextUnformatted(line.c_str());
            }

            if (ImGui::GetScrollY() >= ImGui::GetScrollMaxY()) {
                ImGui::SetScrollHereY(1.f);
            }
        }
        ImGui::EndChild();
    }
    ImGui::End();
}

void Logger::clear() {
    _lines.clear();
    _buffer.str("");
    _buffer.clear();
}

void Logger::flushBuffer() {
    const std::string content = _buffer.str();
    if (!content.empty()) {
        std::istringstream stream(content);
        std::string line;
        while (std::getline(stream, line)) {
            _lines.push_back(line);
        }
        _buffer.str("");
        _buffer.clear();
    }
}
