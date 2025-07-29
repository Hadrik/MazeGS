//
// Created by trric on 7/26/2025.
//

#ifndef DFS_H
#define DFS_H
#include <random>
#include <stack>

#include "GeneratorBase.h"


class DFS final : public GeneratorBase<DFS> {
public:
    DFS();

    static const std::string& getRegisterName();

    void begin(std::unique_ptr<Maze>& maze) override;
    bool step() override;
    void clean() override;
    void drawGUI() override;

private:
    Direction randomDir(const MazeCell *cell);

    int _w = 10;
    int _h = 10;

    MazeCell* _start;

    std::mt19937 _rng;
    std::stack<MazeCell*> _stack;
    Maze* _maze = nullptr;

    static constexpr std::string TAG_visited = "DFSVisited";
};



#endif //DFS_H
