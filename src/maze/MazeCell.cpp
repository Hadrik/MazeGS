//
// Created by trric on 7/19/2025.
//

#include "MazeCell.h"
#include "maze/Maze.h"

Vec2 MazeCell::location() const {
    return {_column, _row};
}

uint32_t MazeCell::column() const {
    return _column;
}

uint32_t MazeCell::row() const {
    return _row;
}

const Infill & MazeCell::infill() const {
    return _infill;
}

Infill & MazeCell::infill() {
    return _infill;
}

void MazeCell::connect(const size_t col, const size_t row) const {
    _parent.connectCells(this->column(), this->row(), col, row);
}

void MazeCell::connect(const MazeCell &other) const {
    _parent.connectCells(*this, other);
}

void MazeCell::connect(const Direction dir) const {
    _parent.connectCells(*this, dir);
}

bool MazeCell::addTag(const std::string& tag) {
    const auto [it, success] = _tags.insert(tag);
    return success;
}

bool MazeCell::hasTag(const std::string &tag) const {
    return _tags.contains(tag);
}

bool MazeCell::removeTag(const std::string &tag) {
    const auto it = _tags.find(tag);
    if (it == _tags.end()) return false;
    _tags.erase(it);
    return true;
}

const std::set<std::string> & MazeCell::getTags() const {
    return _tags;
}

void MazeCell::clearTags() {
    _tags.clear();
}

MazeCell * MazeCell::move(const Direction &dir) const {
    const auto next = Direction::move(location(), dir);
    if (!_parent.positionValid(next)) return nullptr;
    return _parent.cellAt(next);
}
