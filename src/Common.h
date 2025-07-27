//
// Created by trric on 7/18/2025.
//

#ifndef COMMON_H
#define COMMON_H
#include <array>
#include <ostream>
#include <bitset>
#include <cstdint>
#include <raylib-cpp.hpp>

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

    operator Dir() const {
        return direction;
    }

    Vec2 move(Dir dir);
    static Vec2 move(Vec2 pos, Dir dir);
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
    switch (dir) {
        case Direction::NONE: return os << "NONE";
        case Direction::UP: return os << "UP";
        case Direction::RIGHT: return os << "RIGHT";
        case Direction::DOWN: return os << "DOWN";
        case Direction::LEFT: return os << "LEFT";
        default: return os << "UNKNOWN";
    }
}

inline std::ostream& operator<<(std::ostream& os, const Direction& dir) {
    return os << dir.direction;
}

struct Vec2 {
    size_t col, row;

    Vec2() : col(0), row(0) {}
    Vec2(const size_t c, const size_t r) : col(c), row(r) {}

    /// @copybrief Direction::move
    [[nodiscard]] Vec2 move(const Direction d) const {
        return Direction::move(*this, d);
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
};

struct Primitive {
    using enum PrimitiveShape;
    PrimitiveShape shape;
    raylib::Color color;

    Primitive() {
        shape = NONE;
        color = raylib::Color::Blank();
    }

    Primitive(const raylib::Color &c) {
        shape = PrimitiveShape::NONE;
        color = c;
    }

    Primitive(const PrimitiveShape s) {
        shape = s;
        color = raylib::Color::Blank();
    }

    Primitive(const PrimitiveShape s, const raylib::Color& c) {
        shape = s;
        color = c;
    }
};

#endif //COMMON_H
