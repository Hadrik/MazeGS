#include "MazeGS.h"
#include "rlImGui.h"

int main(int, char**)
{
    MazeGS m;

    while (!m.shouldExit()) {
        m.loop();
    }

    return 0;
}
