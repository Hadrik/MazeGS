//
// Created by trric on 7/17/2025.
//

#include "MazeWall.h"

MazeWall::MazeWall() {
    _exists = true;
}

MazeWall::MazeWall(const bool exists) {
    _exists = exists;
}

void MazeWall::set(const bool exists) {
    _exists = exists;
}

MazeWall & MazeWall::operator=(const bool exists) {
    set(exists);
    return *this;
}

bool MazeWall::get() const {
    return _exists;
}

MazeWall::operator bool() const {
    return get();
}

const raylib::Color & MazeWall::getTopColor() const {
    if (_colors.empty()) {
        static const raylib::Color defaultColor = raylib::Color::Black();
        return defaultColor;
    }
    return _colors.back();
}

void MazeWall::setTopColor(const raylib::Color &color) {
    _colors.emplace_back(color);
}

void MazeWall::setTopColor(const Color& color) {
    setTopColor(raylib::Color(color));
}
