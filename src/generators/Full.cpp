//
// Created by trric on 7/22/2025.
//

#include "Full.h"

#include <imgui.h>

#include "Factory.h"

namespace {
    FactoryRegistrar<IGenerator, Full> registrar;
}

const std::string & Full::getRegisterName() {
    static const std::string name = "Full";
    return name;
}

void Full::begin(std::unique_ptr<Maze>& maze) {
    maze = std::make_unique<Maze>(_w, _h);
    maze->cellAt(0, 0)->infill().push(RColor::Yellow());
    maze->cellAt(0, 0)->infill().push(Primitive(Primitive::SQUARE_SMALL, RColor::Black()));
    maze->cellAt(_w-1, _h-1)->infill().push(RColor::Red());
    maze->cellAt(_w-1, _h-1)->infill().push(Primitive::CIRCLE_SMALL);
}

bool Full::step() {
    return true;
}

void Full::drawGUI() {
    ImGui::SliderInt("Width", &_w, 1, 100);
    ImGui::SliderInt("Height", &_h, 1, 100);
}

void Full::clean() {}
