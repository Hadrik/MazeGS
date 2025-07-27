//
// Created by trric on 7/17/2025.
//

#ifndef MAZE_H
#define MAZE_H
#include <vector>

#include "MazeCell.h"
#include "MazeWall.h"


class Maze {
public:
    Maze(size_t width, size_t height, bool fill = true);
    ~Maze() = default;

    MazeCell* cellAt(size_t col, size_t row);
    MazeCell* cellAt(Vec2 pos);
    void connectCells(size_t col1, size_t row1, size_t col2, size_t row2);
    void connectCells(Vec2 pos1, Vec2 pos2);
    void connectCells(const MazeCell& cell1, const MazeCell& cell2);
    void connectCells(const MazeCell& cell, const Direction& dir);

    [[nodiscard]] const std::vector<std::vector<MazeWall>>& getHorizontalWalls() const;
    [[nodiscard]] const std::vector<std::vector<MazeWall>>& getVerticalWalls() const;
    [[nodiscard]] const std::vector<std::vector<MazeCell>>& getCells() const;
    [[nodiscard]] std::vector<std::vector<MazeCell>>& getCells();
    /// [width, height]
    [[nodiscard]] Vec2 getSize() const;
    [[nodiscard]] size_t getWidth() const;
    [[nodiscard]] size_t getHeight() const;

    [[nodiscard]] bool positionValid(size_t col, size_t row) const;
    [[nodiscard]] bool positionValid(int col, int row) const;
    [[nodiscard]] bool positionValid(Vec2 pos) const;

    [[nodiscard]] bool testWall(Vec2 pos, Direction dir) const;
    [[nodiscard]] Directions possibleDirs(const MazeCell& cell, bool wallPossible = true) const;

private:
    std::vector<std::vector<MazeWall>> _hWalls;
    std::vector<std::vector<MazeWall>> _vWalls;
    std::vector<std::vector<MazeCell>> _cells;

    const size_t _width;
    const size_t _height;

    mutable std::mutex _mutex_walls;
    mutable std::mutex _mutex_cells;
};



#endif //MAZE_H
