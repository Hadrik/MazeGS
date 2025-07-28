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

    /// Returns <c>true</c> if the solver is ready to begin
    virtual bool ready() = 0;

    /// Start solving
    virtual void begin(std::unique_ptr<Maze>& maze) = 0;

    /// Call this method repeatedly while its returning <c>std::nopt</c>.
    /// @return <c>std::nopt</c> solving not finished
    /// @return <c>true</c> maze solved
    /// @return <c>false</c> impossible to solve
    virtual std::optional<bool> step() = 0;

    /// Draw ImGui menu with solver settings
    virtual void drawGui() {}
};


#endif //ISOLVER_H
