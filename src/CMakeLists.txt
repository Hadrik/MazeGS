set(PROJECT_SOURCES
        main.cpp
        MazeGS.cpp
        Logger.cpp
        Renderer.cpp
        StepRunner.cpp
        maze/Maze.cpp
        maze/MazeWall.cpp
        maze/MazeCell.cpp
        drawing/Infill.cpp
        generators/Full.cpp
        generators/DFS.cpp
        solvers/PickerTest.cpp
        solvers/RightHand.cpp
)

set(PROJECT_HEADERS
        Common.h
        MazeGS.h
        Renderer.h
        Logger.h
        Factory.h
        StepRunner.h
        ICellPicker.h
        maze/Maze.h
        maze/MazeCell.h
        maze/MazeWall.h
        drawing/Infill.h
        generators/IGenerator.h
        generators/GeneratorBase.h
        generators/Full.h
        generators/DFS.h
        solvers/ISolver.h
        solvers/SolverBase.h
        solvers/PickerTest.h
        solvers/RightHand.h
)

add_executable(${PROJECT_NAME}
        ${PROJECT_SOURCES}
        ${PROJECT_HEADERS}
)

target_include_directories(${PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(${PROJECT_NAME} PRIVATE
        imgui
        raylib
        raylib_cpp
        rlImGui
)