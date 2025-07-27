//
// Created by trric on 7/26/2025.
//

#include "DFS.h"

#include <imgui.h>

#include "Factory.h"

namespace {
    FactoryRegistrar<IGenerator, DFS> registrar;
}

DFS::DFS() {
    std::random_device rd;
    _rng = std::mt19937(rd());
}

const std::string & DFS::getRegisterName() {
    const static std::string name = "DFS";
    return name;
}

void DFS::begin(std::unique_ptr<Maze> &maze) {
    maze = std::make_unique<Maze>(_w, _h, true);
    _maze = maze.get();
    MazeCell* c = _maze->cellAt(
        std::uniform_int_distribution<size_t>(0, _w - 1)(_rng),
        std::uniform_int_distribution<size_t>(0, _h - 1)(_rng)
    );
    c->infill().push(Primitive::CROSS_LARGE);
    _stack.push(c);
}

bool DFS::step() {
    if (_stack.empty()) return true;

    MazeCell* c = _stack.top();
    c->addTag(TAG_visited);
    c->infill().push(RColor::Blue());
    const Direction side = randomDir(c);

    if (side == Direction::NONE) {
        _stack.pop();
        c->infill().push(RColor::Red());
        return false;
    }

    MazeCell* next = c->move(side);
    c->connect(*next);
    _stack.push(next);

    return false;
}

void DFS::drawGUI() {
    ImGui::SliderInt("Width", &_w, 1, 50);
    ImGui::SliderInt("Height", &_h, 1, 50);
}

Direction DFS::randomDir(const MazeCell* cell) {
    Directions d = _maze->possibleDirs(*cell);
    std::vector<Direction> valid;

    std::for_each(Direction::All.begin(), Direction::All.end(), [&](const Direction dir) {
        if (!d.test(dir)) return;
        const Vec2 next = Direction::move(cell->location(), dir);
        if (!_maze->cellAt(next)->hasTag(TAG_visited)) {
            valid.push_back(dir);
        }
    });

    if (valid.empty()) return Direction::NONE;

    std::uniform_int_distribution<size_t> dist(0, valid.size() - 1);
    return valid[dist(_rng)];
}
