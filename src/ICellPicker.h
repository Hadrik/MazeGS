//
// Created by trric on 7/28/2025.
//

#ifndef ICELLPICKER_H
#define ICELLPICKER_H
#include <functional>

#include "Common.h"


class ICellPicker {
public:
    virtual ~ICellPicker() = default;

    using PickCallback = std::function<void(Vec2)>;

    virtual void startPicker(PickCallback cb) = 0;
    virtual bool isPickerActive() = 0;
};


#endif //ICELLPICKER_H
