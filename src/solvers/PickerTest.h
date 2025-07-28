//
// Created by trric on 7/28/2025.
//

#ifndef PICKERTEST_H
#define PICKERTEST_H
#include "ICellPicker.h"
#include "SolverBase.h"


class PickerTest final : public SolverBase<PickerTest> {
public:
    explicit PickerTest(ICellPicker* picker) : _picker(picker) {}

    static const std::string& getRegisterName();

    bool ready() override;
    void begin(std::unique_ptr<Maze> &maze) override;
    std::optional<bool> step() override;

    void drawGui() override;

private:
    Vec2 _pickedPos;

    ICellPicker* _picker;
};



#endif //PICKERTEST_H
