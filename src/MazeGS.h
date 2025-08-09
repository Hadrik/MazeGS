//
// Created by trric on 7/19/2025.
//

#ifndef MAZEGS_H
#define MAZEGS_H
#include <imgui.h>
#include <memory>
#include <maze/Maze.h>

#include "Logger.h"
#include "StepRunner.h"
#include "renderers/GPU/GPU.h"
#include "generators/IGenerator.h"
#include "solvers/ISolver.h"

class MazeGS {
public:
    MazeGS();
    ~MazeGS() = default;

    void loop();

    bool shouldExit() const;

private:
    std::unique_ptr<Maze> _maze = nullptr;
    std::unique_ptr<IGenerator> _generator = nullptr;
    std::unique_ptr<ISolver> _solver = nullptr;
    std::unique_ptr<IRenderer> _renderer = {std::make_unique<GPU>()};
    StepRunner _runner;

    bool _swapRenderer = false;
    std::string _rendererName;

    ImGuiIO* _io;
    Logger& _log;
};



#endif //MAZEGS_H
