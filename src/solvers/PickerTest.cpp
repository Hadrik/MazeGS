//
// Created by trric on 7/28/2025.
//

#include "PickerTest.h"

#include <imgui.h>

#include "Factory.h"

namespace {
    FactoryRegistrar<ISolver, PickerTest> registrar;
}

const std::string & PickerTest::getRegisterName() {
    static const std::string name = "Picker Test";
    return name;
}

void PickerTest::setMaze(Maze *maze) {
}

bool PickerTest::ready() {
    return false;
}

void PickerTest::begin() {
}

std::optional<bool> PickerTest::step() {
    return true;
}

void PickerTest::clean() {
}

void PickerTest::drawGui() {
    if (ImGui::Button("Pick")) {
        _picker->startPicker([this](Vec2 pick) { _pickedPos = pick; });
    }
    if (_picker->isPickerActive()) {
        ImGui::Text("Picker active");
    }
    ImGui::Text("Pos [%d, %d]", _pickedPos.col, _pickedPos.row);
}
