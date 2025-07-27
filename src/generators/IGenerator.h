//
// Created by trric on 7/21/2025.
//

#ifndef IGENERATOR_H
#define IGENERATOR_H
#include <memory>
#include <maze/Maze.h>


class IGenerator {
public:
    virtual ~IGenerator() = default;

    [[nodiscard]] virtual const std::string& getName() const = 0;

    /// Start generation
    virtual void begin(std::unique_ptr<Maze>& maze) = 0;

    /// Call this method repeatedly until it returns <c>true</c> (generation finished)
    virtual bool step() = 0;

    /// Draw ImGui menu with generators settings
    virtual void drawGUI() {}
};


#endif //IGENERATOR_H
