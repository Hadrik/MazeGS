//
// Created by trric on 7/28/2025.
//

#include "RightHand.h"

#include <imgui.h>

#include "Factory.h"

namespace {
    FactoryRegistrar<ISolver, RightHand> registrar;
}

const std::string & RightHand::getRegisterName() {
    static const std::string name = "Right hand";
    return name;
}

void RightHand::setMaze(Maze *maze) {
    _maze = maze;
    if (_startPos.has_value()) setStartPos(_startPos.value());
    if (_endPos.has_value()) setEndPos(_endPos.value());
}

bool RightHand::ready() {
    return _maze && _startPos.has_value() && _endPos.has_value();
}

void RightHand::begin() {
    _position = _startPos.value();
    _direction = _startDir;

    for (auto& col : _maze->getCells()) {
        for (auto& c : col) {
            c.infill().clear();
            c.clearTags();
        }
    }

    _maze->cellAt(_startPos.value())->infill().push(RColor::Green());
    _maze->cellAt(_endPos.value())->infill().push(RColor::Red());
}

std::optional<bool> RightHand::step() {
    MazeCell* cell = _maze->cellAt(_position);

    if (cell->hasTag(dirToTag(_direction))) {
        // If ive already been in this position and direction im going in circles
        cell->infill().push(RColor::Red());
        cell->infill().push(Primitive::CROSS_LARGE);
        return false;
    }
    if (cell->location() == _endPos.value()) {
        cell->infill().push(RColor::Yellow());
        cell->infill().push(Primitive::CIRCLE_LARGE);
        return true;
    }

    // Mark current position as visited in direction
    cell->addTag(dirToTag(_direction));

    const Directions possible = _maze->possibleDirs(*cell, false);
    if (_justTurned) {
        // This could be together with the next if, but it looks better doing just one thing each step
        cell->removeTag(dirToTag(_direction));
        _position = _position.move(_direction);
        _justTurned = false;
    } else if (possible.test(_direction.turnCW())) {
        // If theres no right wall turn right
        cell->removeTag(dirToTag(_direction));
        _direction = _direction.turnCW();
        _justTurned = true;
    } else if (!possible.test(_direction)) {
        // If a wall is in front of me turn left
        _direction = _direction.turnCCW();
    } else {
        // Otherwise just walk forwards
        _position = _position.move(_direction);
    }

    _maze->cellAt(_position)->infill().push(Primitive(_direction));
    _maze->cellAt(_position)->infill().push(RColor::Blue());
    return std::nullopt;
}

void RightHand::clean() {
    std::vector<std::pair<Vec2, Direction>> path;

    _position = _startPos.value();
    _direction = static_cast<Direction>(_maze->cellAt(_position)->infill().topPrimitive());
    while (_position != _endPos.value()) {
        path.emplace_back(_position, _direction);
        _position = _position.move(_direction);
        _direction = static_cast<Direction>(_maze->cellAt(_position)->infill().topPrimitive());
    }

    for (auto& col : _maze->getCells()) {
        for (auto& c : col) {
            c.infill().clear();
            c.clearTags();
        }
    }

    for (const auto& [pos, dir] : path) {
        _maze->cellAt(pos)->infill().push(Primitive(dir));
        _maze->cellAt(pos)->infill().push(RColor::Blue());
    }
    _maze->cellAt(_startPos.value())->infill().push(RColor::Green());
    _maze->cellAt(_startPos.value())->infill().push(Primitive::CIRCLE_LARGE);
    _maze->cellAt(_endPos.value())->infill().push(RColor::Red());
    _maze->cellAt(_endPos.value())->infill().push(Primitive::SQUARE_LARGE);
}

void RightHand::drawGui() {
    ImGui::BeginDisabled(_picker->isPickerActive());
    if (ImGui::Button("Start")) {
        _picker->startPicker([this](Vec2 p) {
            setStartPos(p);
        });
    }
    if (_startPos.has_value()) {
        ImGui::SameLine();
        ImGui::Text("[%d, %d]", _startPos->col, _startPos->row);
    }

    if (ImGui::Button("End")) {
        _picker->startPicker([this](Vec2 p) {
            setEndPos(p);
        });
    }
    if (_endPos.has_value()) {
        ImGui::SameLine();
        ImGui::Text("[%d, %d]", _endPos->col, _endPos->row);
    }

    Direction::ImGuiWidget("Starting direction", _startDir);
    if (_startDir != _lastDir && _startPos.has_value()) {
        setStartPos(_startPos.value());
        _lastDir = _startDir;
    }
    ImGui::EndDisabled();
}

void RightHand::setStartPos(Vec2 p) {
    if (_maze && _startPos.has_value() && _maze->positionValid(_startPos.value())) {
        MazeCell* cell = _maze->cellAt(_startPos.value());
        cell->clearTags();
        cell->infill().clear();
    }
    _startPos = p;
    if (_maze && _maze->positionValid(p)) {
        MazeCell* cell = _maze->cellAt(p);
        cell->infill().push(RColor::Green());
        cell->infill().push(Primitive(_startDir));
    }
}

void RightHand::setEndPos(Vec2 p) {
    if (_maze && _endPos.has_value() && _maze->positionValid(_endPos.value())) {
        MazeCell* cell = _maze->cellAt(_endPos.value());
        cell->clearTags();
        cell->infill().clear();
    }
    _endPos = p;
    if (_maze && _maze->positionValid(p)) {
        _maze->cellAt(p)->infill().push(RColor::Red());
    }
}

std::string RightHand::dirToTag(const Direction& dir) {
    return std::string("RH_") + dir.toString();
}
