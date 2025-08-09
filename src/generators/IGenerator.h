//
// Created by trric on 7/21/2025.
//

#ifndef IGENERATOR_H
#define IGENERATOR_H
#include <memory>
#include <maze/Maze.h>

#include "Factory.h"


class IGenerator : public IFactoryCreatable {
public:
    ~IGenerator() override = default;

    [[nodiscard]] const std::string& getName() const override = 0;

    /// Start generation
    virtual void begin(std::unique_ptr<Maze>& maze) = 0;

    /// Call this method repeatedly until it returns <c>true</c> (generation finished)
    virtual bool step() = 0;

    /// Clean data created by the generation process
    virtual void clean() = 0;

    /// Draw ImGui menu with generator's settings
    virtual void drawGUI() {}
};


#endif //IGENERATOR_H
