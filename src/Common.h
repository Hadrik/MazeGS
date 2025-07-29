//
// Created by trric on 7/18/2025.
//

#ifndef COMMON_H
#define COMMON_H
#include <array>
#include <ostream>
#include <bitset>
#include <cstdint>
#include <imgui.h>
#include <raylib-cpp.hpp>

#include "Logger.h"

struct Vec2;

struct Direction {
    enum Dir : uint8_t {
        NONE = 0,
        UP = 1,
        RIGHT = 2,
        DOWN = 3,
        LEFT = 4,
    };
    Dir direction;

    static std::array<Dir, 4> All;

    Direction(const Dir d) : direction(d) {}

    Direction turnCW() const {
        if (direction == NONE) return NONE;
        return direction == LEFT ? UP : static_cast<Dir>(direction + 1);
    }
    Direction turnCCW() const {
        if (direction == NONE) return NONE;
        return direction == UP ? LEFT : static_cast<Dir>(direction - 1);
    }

    Vec2 move(Dir dir);
    static Vec2 move(Vec2 pos, Dir dir);

    const char* toString() const {
        return toString(direction);
    }
    static const char* toString(const Dir dir) {
        switch (dir) {
            case NONE: return "NONE";
            case UP: return "UP";
            case RIGHT: return "RIGHT";
            case DOWN: return "DOWN";
            case LEFT: return "LEFT";
            default: return "UNKNOWN";
        }
    }

    static void ImGuiWidget(const char* label, Direction& dir) {
        if (ImGui::BeginCombo(label, dir.toString())) {
            for (const Dir d : All) {
                const bool isSelected = dir == d;
                if (ImGui::Selectable(toString(d))) {
                    dir = d;
                }
                if (isSelected) {
                    ImGui::SetItemDefaultFocus();
                }
            }
            ImGui::EndCombo();
        }
    }

    operator Dir() const {
        return direction;
    }
    operator const char* () const {
        return toString();
    }
    bool operator== (const Direction& other) const {
        return direction == other.direction;
    }
    bool operator==(const Direction::Dir &dir) const {
        return direction == dir;
    }
};
inline std::array<Direction::Dir, 4> Direction::All = {UP, RIGHT, DOWN, LEFT};

class Directions : public std::bitset<4> {
public:
    [[nodiscard]] bool test(const Direction::Dir dir) const {
        if (dir == Direction::NONE) {
            return none();
        }
        return std::bitset<4>::test(static_cast<size_t>(dir - 1));
    }

    Directions& set(const Direction::Dir dir, const bool val = true) {
        if (dir != Direction::NONE) {
            std::bitset<4>::set(static_cast<size_t>(dir - 1), val);
        }
        return *this;
    }
};

inline std::ostream& operator<<(std::ostream& os, const Direction::Dir dir) {
    return os << Direction::toString(dir);
}

inline std::ostream& operator<<(std::ostream& os, const Direction& dir) {
    return os << dir.toString();
}

struct Vec2 {
    size_t col, row;

    Vec2() : col(0), row(0) {}
    Vec2(const size_t c, const size_t r) : col(c), row(r) {}

    /// @copybrief Direction::move
    [[nodiscard]] Vec2 move(const Direction d) const {
        return Direction::move(*this, d);
    }

    bool operator==(const Vec2 &other) const {
        return col == other.col && row == other.row;
    }
};
using Point = Vec2;

/// @brief Moves position in direction. Doesn't check if the new position is valid.
inline Vec2 Direction::move(const Vec2 pos, const Dir dir) {
    switch (dir) {
        case Dir::UP:
            return {pos.col, pos.row - 1};
        case Dir::RIGHT:
            return {pos.col + 1, pos.row};
        case Dir::DOWN:
            return {pos.col, pos.row + 1};
        case Dir::LEFT:
            return {pos.col - 1, pos.row};
        case Dir::NONE:
            return {pos.col, pos.row};
    }
    return {};
}


enum class PrimitiveShape {
    NONE = 0,
    SQUARE_SMALL,
    SQUARE_LARGE,
    TRIANGLE_SMALL,
    TRIANGLE_LARGE,
    CIRCLE_SMALL,
    CIRCLE_LARGE,
    CROSS_SMALL,
    CROSS_LARGE,
    ARROW_UP,
    ARROW_RIGHT,
    ARROW_DOWN,
    ARROW_LEFT,
};

struct Primitive {
    using enum PrimitiveShape;
    PrimitiveShape shape;
    raylib::Color color;

    Primitive() : Primitive(NONE, RColor::Black()) {}
    Primitive(const PrimitiveShape s) : Primitive(s, RColor::Black()) {}
    Primitive(const PrimitiveShape s, const raylib::Color& c) : shape(s), color(c) {}
    explicit Primitive(const Direction& dir) : Primitive() {
        switch (dir) {
            case Direction::UP:
                shape = ARROW_UP;
            break;
            case Direction::RIGHT:
                shape = ARROW_RIGHT;
            break;
            case Direction::DOWN:
                shape = ARROW_DOWN;
            break;
            case Direction::LEFT:
                shape = ARROW_LEFT;
            break;
            default:
                shape = NONE;
        }
    }
    Primitive(const Direction& dir, const raylib::Color& c) : Primitive(dir) {
        color = c;
    }

    explicit operator Direction() const {
        switch (shape) {
            case ARROW_UP:
                return Direction::UP;
            case ARROW_RIGHT:
                return Direction::RIGHT;
            case ARROW_DOWN:
                return Direction::DOWN;
            case ARROW_LEFT:
                return Direction::LEFT;
            default:
                return Direction::NONE;
        }
    }
};

#endif //COMMON_H
