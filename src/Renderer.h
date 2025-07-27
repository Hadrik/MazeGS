//
// Created by trric on 7/19/2025.
//

#ifndef RENDERER_H
#define RENDERER_H

#include <imgui.h>
#include <raylib-cpp.hpp>

#include "maze/Maze.h"

class Renderer {
public:
    Renderer() = default;
    ~Renderer() = default;

    static const raylib::Window& InitWindow();
    static void newFrame();
    void render(const Maze* maze);

private:
    raylib::Image drawMaze(const Maze* maze, const ImVec2& maxSize);
    void drawCells(const Maze* maze, raylib::Image& img) const;
    static void drawCellInfill(raylib::Image& img, const raylib::Rectangle& bounds, const MazeCell& cell) ;
    void drawGrid(const Maze* maze, raylib::Image& img) const;

    [[nodiscard]] static raylib::Rectangle scaleRect(const raylib::Rectangle& rect, float scale) ;

    int _cellSize;

    static raylib::Window _window;
};



#endif //RENDERER_H
