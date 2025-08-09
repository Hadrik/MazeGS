//
// Created by trric on 7/19/2025.
//

#ifndef RENDERER_H
#define RENDERER_H

#include <imgui.h>
#include <map>
#include <optional>
#include <raylib-cpp.hpp>

#include "maze/Maze.h"
#include "../IRenderer.h"

class CPU final : public IRenderer {
public:
    CPU();
    ~CPU() override;

    void newFrame() override;
    void render(const Maze* maze) override;
    bool shouldExit() override;

    void startPicker(const PickCallback cb) override { _pickCallback = cb; _pickerActive = true; }
    bool isPickerActive() override { return _pickerActive; }

    [[nodiscard]] const std::string & getName() const override { return getRegisterName(); }
    [[nodiscard]] static const std::string& getRegisterName() {
        static std::string registerName("CPU");
        return registerName;
    }

private:
    std::optional<Vec2> handleTilePicking(const Maze* maze = nullptr);
    void createInfills();

    raylib::Image drawMaze(const Maze* maze, const ImVec2& maxSize);
    void drawCells(const Maze* maze, raylib::Image& img) const;
    void drawCellInfill(raylib::Image& img, const raylib::Rectangle& bounds, const MazeCell& cell) const;
    void drawGrid(const Maze* maze, raylib::Image& img) const;

    [[nodiscard]] static raylib::Rectangle scaleRect(const raylib::Rectangle& rect, float scale);

    int _cellSize = 0;
    bool _pickerActive = false;
    PickCallback _pickCallback;
    ImVec2 _lastImgPos;
    ImVec2 _lastImgSize;
    std::map<int, raylib::Image> _infills;
    const float _infillSize = 100.f;
};



#endif //RENDERER_H
