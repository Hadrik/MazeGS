//
// Created by trric on 7/30/2025.
//

#ifndef IRENDERER_H
#define IRENDERER_H

#include "Factory.h"
#include "maze/Maze.h"


class IRenderer : public IFactoryCreatable, public ICellPicker {
public:
    ~IRenderer() override = default;

    virtual void newFrame() = 0;
    virtual void render(const Maze* maze) = 0;

    virtual bool shouldExit() = 0;
};



#endif //IRENDERER_H
