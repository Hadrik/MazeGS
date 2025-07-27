#include "MazeGS.h"
#include "raylib-cpp.hpp"
#include "Renderer.h"
#include "rlImGui.h"

int main(int, char**)
{
    const raylib::Window& win = Renderer::InitWindow();

    MazeGS m;

    while (!win.ShouldClose()) {
        m.loop();
    }

    rlImGuiShutdown();
    CloseWindow();
    return 0;
}
