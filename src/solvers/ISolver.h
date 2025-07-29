//
// Created by trric on 7/21/2025.
//

#ifndef ISOLVER_H
#define ISOLVER_H
#include <memory>
#include <optional>
#include <string>
#include <maze/Maze.h>


class ISolver {
public:
    virtual ~ISolver() = default;

    [[nodiscard]] virtual const std::string& getName() const = 0;

    /// Set maze to solve
    virtual void setMaze(Maze* maze) = 0;

    /// Returns <c>true</c> if the solver is ready to begin
    virtual bool ready() = 0;

    /// Start solving
    virtual void begin() = 0;

    /// Call this method repeatedly
    /// @return <c>std::nullopt</c> solving not finished
    /// @return <c>true</c> maze solved
    /// @return <c>false</c> impossible to solve
    virtual std::optional<bool> step() = 0;

    /// Clean any extra data from solving and leave only the found path
    virtual void clean() = 0;

    /// Draw ImGui menu with solver settings
    virtual void drawGui() {}
};


#endif //ISOLVER_H
