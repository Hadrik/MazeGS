//
// Created by trric on 7/19/2025.
//

#include "CPU.h"

#include <imgui.h>
#include <algorithm>

#include "rlImGui.h"
#include "Logger.h"

namespace {
    FactoryRegistrar<IRenderer, CPU> registrar;
}

CPU::CPU() {
    createInfills();
    SetConfigFlags(FLAG_MSAA_4X_HINT | FLAG_VSYNC_HINT | FLAG_WINDOW_RESIZABLE);
    InitWindow(1200, 800, "MazeGS");
    SetTargetFPS(165);
    SetTraceLogLevel(LOG_ERROR);
    SetExitKey(KEY_NULL);

    rlImGuiSetup(true);

    ImGui::GetIO().ConfigFlags |= ImGuiConfigFlags_DockingEnable;
    ImGui::GetIO().ConfigWindowsMoveFromTitleBarOnly = true;
    ImGui::GetIO().ConfigDebugIsDebuggerPresent = true;
}

CPU::~CPU() {
    rlImGuiShutdown();
    CloseWindow();
}

void CPU::newFrame() {
    BeginDrawing();
    rlImGuiBegin();
    ImGui::DockSpaceOverViewport();
}

void CPU::render(const Maze* maze) {
    // Works
    ImGui::Begin("Maze");
    const ImVec2 size = ImGui::GetContentRegionAvail();
    const raylib::Image img = drawMaze(maze, size);
    const raylib::Texture texture = img.LoadTexture();

    if (texture.IsValid()) {
        const auto width = static_cast<float>(img.width), height = static_cast<float>(img.height);
        const float scale = std::min(size.x / width, size.y / height);
        const auto scaledWidth = static_cast<int>(width * scale), scaledHeight = static_cast<int>(height * scale);
        rlImGuiImageSize(&texture, scaledWidth, scaledHeight);
        _lastImgPos = ImGui::GetItemRectMin();
        _lastImgSize = ImVec2(scaledWidth, scaledHeight);
        const auto res = handleTilePicking(maze);
        if (res.has_value()) {
            ImGui::SetTooltip("[%d, %d]", res.value().col, res.value().row);
        }
    } else {
        ImGui::Text("No data");
    }
    ImGui::End();

    Logger::get().drawWindow();
    rlImGuiEnd();
    EndDrawing();
}

bool CPU::shouldExit() {
    return WindowShouldClose();
}

std::optional<Vec2> CPU::handleTilePicking(const Maze* maze) {
    if (!_pickerActive) return std::nullopt;
    if (raylib::Keyboard::IsKeyDown(KEY_ESCAPE)) {
        _pickerActive = false;
        return std::nullopt;
    }
    if (_cellSize == 0) return std::nullopt;

    Vec2 result;

    const ImVec2 mousePos = ImGui::GetMousePos();
    const ImVec2 imgMin = _lastImgPos;
    const ImVec2 imgMax(_lastImgPos.x + _lastImgSize.x, _lastImgPos.y + _lastImgSize.y);

    if (mousePos.x < imgMin.x || mousePos.x > imgMax.x ||
        mousePos.y < imgMin.y || mousePos.y > imgMax.y) {
        return std::nullopt;
    }

    const ImVec2 relativePos(mousePos.x - imgMin.x, mousePos.y - imgMin.y);
    result.col = static_cast<size_t>(relativePos.x) / _cellSize;
    result.row = static_cast<size_t>(relativePos.y) / _cellSize;

    if (maze) {
        result.col = std::clamp(result.col, static_cast<size_t>(0), maze->getWidth() - 1);
        result.row = std::clamp(result.row, static_cast<size_t>(0), maze->getHeight() - 1);
    }

    if (ImGui::IsMouseClicked(ImGuiMouseButton_Left) && _pickCallback) {
        _pickerActive = false;
        _pickCallback(result);
    }

    return result;
}

raylib::Image CPU::drawMaze(const Maze *maze, const ImVec2& maxSize) {
    if (!maze) {
        raylib::Image img(100, 10, RColor::Blank());
        img.DrawText("No maze generated", 1, 0, 10, RColor::White());
        return img;
    }

    const int mazeW = static_cast<int>(maze->getWidth());
    const int mazeH = static_cast<int>(maze->getHeight());
    _cellSize = std::min(static_cast<int>(maxSize.x) / mazeW, static_cast<int>(maxSize.y) / mazeH);
    const int totalW = mazeW * _cellSize;
    const int totalH = mazeH * _cellSize;

    raylib::Image img(totalW, totalH);
    img.ClearBackground(RColor::White());

    drawCells(maze, img);
    drawGrid(maze, img);

    // Outline
    img.DrawRectangleLines({0, 0, static_cast<float>(img.width), static_cast<float>(img.height)}, 2, RColor::LightGray());

    return img;
}

void CPU::drawCells(const Maze *maze, raylib::Image &img) const {
    const auto& cells = maze->getCells();
    for (const auto& col : cells) {
        for (const auto& cell : col) {
            const raylib::Vector2 pos(
                cell.column() * _cellSize,
                cell.row() * _cellSize
            );
            const raylib::Rectangle cellBounds(pos, raylib::Vector2(_cellSize, _cellSize));
            drawCellInfill(img, cellBounds, cell);
        }
    }
}

void CPU::createInfills() {
    // Square
    RImage square(_infillSize, _infillSize, RColor::Blank());
    square.DrawRectangleLines({0, 0, _infillSize, _infillSize}, 4, RColor::Black());
    _infills[0] = square;

    // Circle
    RImage circle(_infillSize, _infillSize, RColor::Blank());
    ImageDrawCircleLines(&circle, _infillSize / 2, _infillSize / 2, _infillSize / 2, RColor::Black());
    _infills[1] = circle;

    // Cross
    RImage cross(_infillSize, _infillSize, RColor::Blank());
    cross.DrawLine(0, 0, _infillSize, _infillSize, RColor::Black());
    cross.DrawLine(0, _infillSize, _infillSize, 0, RColor::Black());
    _infills[2] = cross;

    // Triangle
    RImage triangle(_infillSize, _infillSize, RColor::Blank());
    triangle.DrawLine(_infillSize / 2, 0, 0, _infillSize, RColor::Black());
    triangle.DrawLine(_infillSize / 2, 0, _infillSize, _infillSize, RColor::Black());
    triangle.DrawLine(0, _infillSize, _infillSize, _infillSize, RColor::Black());
    _infills[3] = triangle;

    // Arrows
    RImage arrowUp(_infillSize, _infillSize, RColor::Blank());
    arrowUp.DrawLine(_infillSize / 2, 0, _infillSize / 2, _infillSize, RColor::Black());
    arrowUp.DrawLine(_infillSize / 2, 0, _infillSize / 4, _infillSize / 4, RColor::Black());
    arrowUp.DrawLine(_infillSize / 2, 0, _infillSize * 0.75f, _infillSize / 4, RColor::Black());
    _infills[4] = arrowUp;

    RImage arrowRight = arrowUp;
    arrowRight.RotateCW();
    _infills[5] = arrowRight;

    RImage arrowDown = arrowUp;
    arrowDown.FlipVertical();
    _infills[6] = arrowDown;

    RImage arrowLeft = arrowUp;
    arrowLeft.RotateCCW();
    _infills[7] = arrowLeft;
}

void CPU::drawCellInfill(raylib::Image &img, const raylib::Rectangle &bounds, const MazeCell &cell) const {
    const auto& [color, prim] = cell.infill().topPair();
    img.DrawRectangle(bounds, color);
    const auto rs = scaleRect(bounds, 0.4f);
    const auto rl = scaleRect(bounds, 0.8f);
    const auto ris = Rectangle(0, 0, _infillSize, _infillSize);

    switch (prim.shape) {
        case Primitive::SQUARE_SMALL:
            img.Draw(_infills.at(0), ris, rs);
        break;
        case Primitive::SQUARE_LARGE:
            img.Draw(_infills.at(0), ris, rl);
        break;
        case Primitive::CIRCLE_SMALL:
            img.Draw(_infills.at(1), ris, rs);
        break;
        case Primitive::CIRCLE_LARGE:
            img.Draw(_infills.at(1), ris, rl);
        break;
        case Primitive::CROSS_SMALL:
            img.Draw(_infills.at(2), ris, rs);
        break;
        case Primitive::CROSS_LARGE:
            img.Draw(_infills.at(2), ris, rl);
        break;
        case PrimitiveShape::TRIANGLE_SMALL:
            img.Draw(_infills.at(3), ris, rs);
        break;
        case PrimitiveShape::TRIANGLE_LARGE:
            img.Draw(_infills.at(3), ris, rl);
        break;
        case PrimitiveShape::ARROW_UP:
            img.Draw(_infills.at(4), ris, rl);
        break;
        case PrimitiveShape::ARROW_RIGHT:
            img.Draw(_infills.at(5), ris, rl);
        break;
        case PrimitiveShape::ARROW_DOWN:
            img.Draw(_infills.at(6), ris, rl);
        break;
        case PrimitiveShape::ARROW_LEFT:
            img.Draw(_infills.at(7), ris, rl);
        default:
        break;
    }
}

void CPU::drawGrid(const Maze *maze, raylib::Image &img) const {
    const auto& hWalls = maze->getHorizontalWalls();
    for (size_t col = 0; col < hWalls.size(); ++col) {
        for (size_t row = 0; row < hWalls.at(0).size(); ++row) {
            const MazeWall& w = hWalls.at(col).at(row);
            if (!w) continue;

            const Vector2 start = {.x = static_cast<float>(col * _cellSize), .y = static_cast<float>((row + 1) * _cellSize)};
            const Vector2 end = {.x = static_cast<float>((col + 1) * _cellSize), .y = static_cast<float>((row + 1) * _cellSize)};
            img.DrawLine(start, end, 4, w.getTopColor());
        }
    }

    const auto& vWalls = maze->getVerticalWalls();
    for (size_t col = 0; col < vWalls.size(); ++col) {
        for (size_t row = 0; row < vWalls.at(0).size(); ++row) {
            const MazeWall& w = vWalls.at(col).at(row);
            if (!w) continue;

            const Vector2 start = {.x = static_cast<float>((col + 1) * _cellSize), .y = static_cast<float>(row * _cellSize)};
            const Vector2 end = {.x = static_cast<float>((col + 1) * _cellSize), .y = static_cast<float>((row + 1) * _cellSize)};
            img.DrawLine(start, end, 4, w.getTopColor());
        }
    }
}

raylib::Rectangle CPU::scaleRect(const raylib::Rectangle &rect, const float scale) {
    const float newWidth = rect.width * scale;
    const float newHeight = rect.height * scale;

    const float offsetX = (rect.width - newWidth) / 2.0f;
    const float offsetY = (rect.height - newHeight) / 2.0f;

    return {
        rect.x + offsetX,
        rect.y + offsetY,
        newWidth,
        newHeight
    };
}

