//
// Created by trric on 7/17/2025.
//

#include "Maze.h"

#include "Logger.h"

Maze::Maze(const size_t width, const size_t height, const bool fill) : _width(width), _height(height) {
    Logger::get() << "Creating full maze - Width: " << width << ", Height: " << height << std::endl;
    if (width <= 0 || height <= 0) {
        Logger::get() << "Cannot create a maze with a 0 size" << std::endl;
        return;
    }

    // Create cells - column major
    // _cells = std::vector(width, std::vector(height, MazeCell(*this)));
    _cells.reserve(width);
    for (size_t col = 0; col < width; col++) {
        std::vector<MazeCell> cells;
        for (size_t row = 0; row < height; row++) {
            cells.emplace_back(*this, col, row);
        }
        _cells.push_back(cells);
    }

    // Create walls
    // Dont create a horizontal wall if maze only has one row
    if (height == 1) {
        _hWalls = std::vector<std::vector<MazeWall>>();
    } else {
        _hWalls = std::vector(width, std::vector(height - 1, MazeWall(fill)));
    }
    _vWalls = std::vector(width - 1, std::vector(height, MazeWall(fill)));
}

MazeCell* Maze::cellAt(const size_t col, const size_t row) {
    try {
        MazeCell& cell = _cells.at(col).at(row);
        return &cell;
    } catch (const std::out_of_range&) {
        Logger::get() << "Invalid maze cell access - row: " << row << " col: " << col << std::endl;
    }
    return nullptr;
}

MazeCell* Maze::cellAt(const Vec2 pos) {
    return cellAt(pos.col, pos.row);
}

void Maze::connectCells(const size_t col1, const size_t row1, const size_t col2, const size_t row2) {
    if (std::abs(static_cast<int>(col1) - static_cast<int>(col2)) + std::abs(static_cast<int>(row1) - static_cast<int>(row2)) != 1) {
        Logger::get() << "Trying to connect non-neighbouring cells - [" << col1 << "," << row1 << "] - [" << col2 << "," << row2 << "]" << std::endl;
        return;
    }

    MazeWall* wall;
    if (row1 != row2) {
        // different rows - break horizontal wall
        wall = &_hWalls.at(col1).at(std::min(row1, row2));
    } else {
        wall = &_vWalls.at(std::min(col1, col2)).at(row1);
    }

    // if (const size_t left = std::min(row1, row2); left != std::max(row1, row2)) {
    //     wall = &_hWalls.at(col1).at(left);
    // } else {
    //     const size_t top = std::min(col1, col2);
    //     wall = &_vWalls.at(top).at(row1);
    // }
    if (wall->get() == false) {
        Logger::get() << "Trying to break already broken wall - [" << col1 << "," << row1 << "] - [" << col2 << "," << row2 << "]" << std::endl;
        return;
    }
    wall->set(false);
}

void Maze::connectCells(const Vec2 pos1, const Vec2 pos2) {
    connectCells(pos1.col, pos1.row, pos2.col, pos2.row);
}

void Maze::connectCells(const MazeCell &cell1, const MazeCell &cell2) {
    connectCells(cell1.column(), cell1.row(), cell2.column(), cell2.row());
}

void Maze::connectCells(const MazeCell &cell, const Direction &dir) {
    const auto next = Direction::move(cell.location(), dir);
    if (!positionValid(next)) {
        Logger::get() << "Trying to connect cells by direction with invalid result - [" << cell.column() << ", " << cell.row() << "]: " << dir << std::endl;
        return;
    }
    connectCells(cell.location(), next);
}

const std::vector<std::vector<MazeWall>> & Maze::getHorizontalWalls() const {
    return _hWalls;
}

const std::vector<std::vector<MazeWall>> & Maze::getVerticalWalls() const {
    return _vWalls;
}

const std::vector<std::vector<MazeCell>> & Maze::getCells() const {
    return _cells;
}

Vec2 Maze::getSize() const {
    return {getWidth(), getHeight()};
}

size_t Maze::getWidth() const {
    return _width;
}

size_t Maze::getHeight() const {
    return _height;
}

bool Maze::positionValid(const size_t col, const size_t row) const {
    return col < getWidth() && row < getHeight();
}

bool Maze::positionValid(const int col, const int row) const {
    return 0 <= col && col < getWidth() &&
           0 <= row && row < getHeight();
}

bool Maze::positionValid(const Vec2 pos) const {
    return positionValid(pos.col, pos.row);
}

bool Maze::testWall(const Vec2 pos, const Direction dir) const {
    switch (dir) {
        case Direction::UP:
            if (pos.row == 0) return true;
        return _hWalls.at(pos.col).at(pos.row - 1).get();
        case Direction::DOWN:
            if (pos.row == getHeight() - 1) return true;
        return _hWalls.at(pos.col).at(pos.row).get();
        case Direction::LEFT:
            if (pos.col == 0) return true;
        return _vWalls.at(pos.col - 1).at(pos.row).get();
        case Direction::RIGHT:
            if (pos.col == getWidth() - 1) return true;
        return _vWalls.at(pos.col).at(pos.row).get();
        default:
            return true;
    }
}

Directions Maze::possibleDirs(const MazeCell &cell, const bool wallPossible) const {
    Directions dirs;
    const Vec2 pos = cell.location();

    if (pos.row > 0 && (wallPossible || !testWall(pos, Direction::UP))) {
        dirs.set(Direction::UP);
    }

    if (pos.row < getHeight() - 1 && (wallPossible || !testWall(pos, Direction::DOWN))) {
        dirs.set(Direction::DOWN);
    }

    if (pos.col > 0 && (wallPossible || !testWall(pos, Direction::LEFT))) {
        dirs.set(Direction::LEFT);
    }

    if (pos.col < getWidth() - 1 && (wallPossible || !testWall(pos, Direction::RIGHT))) {
        dirs.set(Direction::RIGHT);
    }

    return dirs;
}
