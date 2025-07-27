//
// Created by trric on 7/19/2025.
//

#ifndef MAZECELL_H
#define MAZECELL_H
#include <string>
#include <set>

#include "drawing/Infill.h"

class Maze;

class MazeCell {
public:
    explicit MazeCell(Maze& parentMaze, const uint32_t col, const uint32_t row) : _parent(parentMaze), _column(col), _row(row) {};
    MazeCell() = delete;
    MazeCell(const MazeCell& other) = delete;
    MazeCell(MazeCell&& other) noexcept;
    ~MazeCell() = default;

    [[nodiscard]] Vec2 location() const;
    [[nodiscard]] uint32_t column() const;
    [[nodiscard]] uint32_t row() const;

    [[nodiscard]] const Infill& infill() const;
    [[nodiscard]] Infill& infill();

    void connect(size_t col, size_t row) const;
    void connect(const MazeCell& other) const;
    void connect(Direction dir) const;

    bool addTag(const std::string& tag);
    [[nodiscard]] bool hasTag(const std::string& tag) const;
    bool removeTag(const std::string& tag);
    [[nodiscard]] const std::set<std::string>& getTags() const;
    void clearTags();

    [[nodiscard]] MazeCell* move(const Direction& dir) const;

private:
    Maze& _parent;
    const uint32_t _column, _row;

    Infill _infill = {};
    std::set<std::string> _tags = {};

    mutable std::mutex _mutex_tags;
};



#endif //MAZECELL_H
