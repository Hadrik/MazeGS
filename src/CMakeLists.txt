set(PROJECT_SOURCES
        main.cpp
        MazeGS.cpp
        Logger.cpp
        Renderer.cpp
        maze/Maze.cpp
        maze/MazeWall.cpp
        maze/MazeCell.cpp
        drawing/Infill.cpp
        generators/Full.cpp
)

set(PROJECT_HEADERS
        Common.h
        MazeGS.h
        Renderer.h
        Logger.h
        Factory.h
        maze/MazeCell.h
        drawing/Infill.h
        solvers/ISolver.h
        generators/IGenerator.h
        generators/GeneratorBase.h
        generators/Full.h
        StepRunner.cpp
        StepRunner.h
        generators/DFS.cpp
        generators/DFS.h
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