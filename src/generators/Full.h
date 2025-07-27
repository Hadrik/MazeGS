//
// Created by trric on 7/22/2025.
//

#ifndef FULL_H
#define FULL_H
#include "GeneratorBase.h"


class Full final : public GeneratorBase<Full> {
public:
    static const std::string& getRegisterName();

    void begin(std::unique_ptr<Maze>& maze) override;
    bool step() override;
    void drawGUI() override;
    void clean() override;

private:
    int _w = 10;
    int _h = 10;
};



#endif //FULL_H
