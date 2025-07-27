//
// Created by trric on 7/19/2025.
//

#ifndef MAZEGS_H
#define MAZEGS_H
#include <imgui.h>
#include <memory>
#include <maze/Maze.h>

#include "Logger.h"
#include "Renderer.h"
#include "StepRunner.h"
#include "generators/IGenerator.h"
#include "solvers/ISolver.h"

class MazeGS {
public:
    explicit MazeGS() : _io(ImGui::GetIO()), _log(Logger::get()) {}

    ~MazeGS() = default;

    void loop();

private:
    std::unique_ptr<Maze> _maze = nullptr;
    std::unique_ptr<IGenerator> _generator = nullptr;
    std::unique_ptr<ISolver> _solver = nullptr;
    StepRunner _runner;
    Renderer _renderer {};

    ImGuiIO& _io;
    Logger& _log;
};



#endif //MAZEGS_H
