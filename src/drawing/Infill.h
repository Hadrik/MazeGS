//
// Created by trric on 7/19/2025.
//

#ifndef INFILL_H
#define INFILL_H
#include <mutex>
#include <variant>

#include "Common.h"


class Infill {
public:
    enum class ContainedType {
        Color,
        Primitive
    };

    Infill() = default;
    Infill(const Infill& other);
    Infill(Infill&& other) noexcept;
    ~Infill() = default;
    Infill& operator=(Infill&& other) noexcept;

    void push(const raylib::Color& color);
    void push(const Primitive& primitive);

    bool pop();
    bool pop(ContainedType type);

    void clear();

    [[nodiscard]] const std::variant<raylib::Color, Primitive>& top() const;
    [[nodiscard]] const raylib::Color& topColor() const;
    [[nodiscard]] const Primitive& topPrimitive() const;
    [[nodiscard]] std::pair<raylib::Color, Primitive> topPair() const;

private:
    std::vector<std::variant<raylib::Color, Primitive>> _stack;
    mutable std::mutex _mutex = {};
};



#endif //INFILL_H
