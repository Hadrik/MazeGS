//
// Created by trric on 7/28/2025.
//

#ifndef RIGHTHAND_H
#define RIGHTHAND_H
#include "ICellPicker.h"
#include "SolverBase.h"
#include "Common.h"


class RightHand final : public SolverBase<RightHand> {
public:
    explicit RightHand(ICellPicker* picker) : _picker(picker) {}

    static const std::string& getRegisterName();

    void setMaze(Maze *maze) override;
    bool ready() override;
    void begin() override;
    std::optional<bool> step() override;
    void clean() override;

    void drawGui() override;

    void setStartPos(Vec2 p);
    void setEndPos(Vec2 p);

private:
    ICellPicker* _picker;
    Maze* _maze = nullptr;

    std::optional<Vec2> _startPos = std::nullopt;
    std::optional<Vec2> _endPos = std::nullopt;
    Direction _startDir = Direction::UP;
    Direction _lastDir = Direction::UP; // To redraw arrow when new direction is selected

    Vec2 _position;
    Direction _direction = Direction::NONE;
    bool _justTurned = false;

    static std::string dirToTag(const Direction& dir);
};



#endif //RIGHTHAND_H
